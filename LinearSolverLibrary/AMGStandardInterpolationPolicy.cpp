#include "pch.h"

#include "AMGStandardInterpolationPolicy.h"
#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableCategorizer.h"
#include "VariableInfluenceAccessor.h"
#include "AMGStandardSplitting.h"
#include "AMGCoarseVariableIndexer.h"

using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGStandardInterpolationPolicy::AMGStandardInterpolationPolicy() {}

bool
AMGStandardInterpolationPolicy::Generate(SparseMatrix2D const & m) {
    AMGStandardCoarseningStrengthPolicy strength_policy{m};
    variable_categorizer_ = std::make_unique<VariableCategorizer>(m.rows());
    VariableInfluenceAccessor influence_accessor{strength_policy, *variable_categorizer_};
    AMGStandardSplitting splitting{m, influence_accessor, *variable_categorizer_};
    splitting.generateSplitting();
    if (ComputeInterpolationOperator(m, strength_policy, *variable_categorizer_) == false)
        return false;
    ComputeRestrictionOperator(interpolation_operator_);
    ComputeGalerkinOperator(m, interpolation_operator_, restriction_operator_);
    return true;
}

bool
AMGStandardInterpolationPolicy::ComputeInterpolationOperator(SparseMatrix2D const & m, AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) {
    // The interpolation operator has one row for each variable (i.e. both
    // fine and coarse) and one column for each coarse variable.
    Interpolation_t interpolation_op;
    AMGCoarseVariableIndexer indexer{variable_categorizer};
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m);
    while (row_it) {
        auto fine_variable = row_it.row();
        if (variable_categorizer.GetType(fine_variable) == VariableCategorizer::Type::COARSE) {
            // no interpolation needed
            auto cv = indexer.Index(fine_variable);
            interpolation_op[{fine_variable, cv}] = 1;
        }
        else {
            auto interpolatory_set = strength_policy.GetInfluencedByVariables(fine_variable);
            if (interpolatory_set->size() == 0) {
                // Fine variable 'fine_variable' has no string connections to any other coarse variable.
                // This means that it can be solved for directly as it has no dependencies on any other
                // variables. We therefore stop coarsening at this level and solve via direct solve (LU).
                // Alternatively, the fine variable could be removed from the system. Then, we continue
                // coarsening with the reduced system.
//                m.print();
                return false;
            }

            using size_type = IMatrix2D::size_type;

            // generate \hat{a}_{ij} set
            std::map<size_type, double> a_i_hat;


            // extract the a_ik from i's direct neighborhood N_i
            auto column_iterator = *row_it;
            for (; column_iterator; ++column_iterator) {
                auto k = column_iterator.column();
                double a_ik = m(fine_variable, k);
                a_i_hat[k] = a_ik;
            }

            std::set<size_type> extended_coarse_variable_set;

            // replace all e_j where j is a fine variable
            for (auto j : *interpolatory_set) {
                if (variable_categorizer.GetType(j) != VariableCategorizer::Type::FINE) {
                    if (variable_categorizer.GetType(j) == VariableCategorizer::Type::COARSE)
                        extended_coarse_variable_set.insert(j);
                    continue;
                }

                auto j_interpolatory_set = strength_policy.GetInfluencedByVariables(j);
                for (auto k : *j_interpolatory_set) {
                    if (variable_categorizer.GetType(k) == VariableCategorizer::Type::COARSE)
                        extended_coarse_variable_set.insert(k);
                }


                double a_jj = m(j, j);
                double fac = 1.0 / a_jj;

                ConstRowColumnIterator<SparseMatrix2D> j_row_it = MatrixIterators::getConstRowColumnIterator(m, j);
                auto j_column_iterator = *j_row_it;
                for (; j_column_iterator; ++j_column_iterator) {
                    auto k = j_column_iterator.column();
                    double a_jk = m(j, k);
                    a_i_hat[k] += fac * a_jk;
                }
            }


            double a_num = 0;
            double a_denum = 0;
            double b_num = 0;
            double b_denum = 0;
            double diag = 0;

            for (auto coarse_variable : extended_coarse_variable_set) {
                BOOST_ASSERT_MSG(variable_categorizer.GetType(coarse_variable) == VariableCategorizer::Type::COARSE, "AMGStandardInterpolationPolicy::ComputeInterpolationOperator: Expected coarse variable");

                BOOST_ASSERT_MSG(a_i_hat.find(coarse_variable) != std::end(a_i_hat), "AMGStandardInterpolationPolicy::ComputeInterpolationOperator: Couldn't find matrix element");

                double a_ik = a_i_hat[coarse_variable];
                if (a_ik < 0)
                    a_denum += a_ik;
                else
                    b_denum += a_ik;
            }

            column_iterator = *row_it;
            for (; column_iterator; ++column_iterator) {
                auto k = column_iterator.column();

                BOOST_ASSERT_MSG(a_i_hat.find(k) != std::end(a_i_hat), "AMGStandardInterpolationPolicy::ComputeInterpolationOperator: Couldn't find matrix element");

                double a_ik = a_i_hat[k];
                if (fine_variable == k)
                    diag += a_ik;
                else if (a_ik < 0)
                    a_num += a_ik;
                else
                    b_num += a_ik;
            }

            double alpha = a_num / a_denum;
            double beta;

            if (b_denum == 0) {
                // add positive a_ik to diagonal
                diag += b_num;
                beta = 0;
            } else
                beta = b_num / b_denum;

            double neg_coeff = -alpha / diag;
            double pos_coeff = -beta / diag;

            for (auto coarse_variable : extended_coarse_variable_set) {
                BOOST_ASSERT_MSG(variable_categorizer.GetType(coarse_variable) == VariableCategorizer::Type::COARSE, "AMGStandardInterpolationPolicy::ComputeInterpolationOperator: Expected coarse variable");

                double w_ik;

                BOOST_ASSERT_MSG(a_i_hat.find(coarse_variable) != std::end(a_i_hat), "AMGStandardInterpolationPolicy::ComputeInterpolationOperator: Couldn't find matrix element");

                double a_ik = a_i_hat[coarse_variable];
                if (a_ik < 0)
                    w_ik = neg_coeff * a_ik;
                else
                    w_ik = pos_coeff * a_ik;
                auto cv = indexer.Index(coarse_variable);
                interpolation_op[{fine_variable, cv}] = w_ik;
            }
        }
        ++row_it;
    }
    CreateInterpolationOperator(m.rows(), indexer.NumberOfVariables(), interpolation_op);
    return true;
}

void
AMGStandardInterpolationPolicy::CreateInterpolationOperator(size_type rows, size_type columns, Interpolation_t const & interpolation_op) {
    interpolation_operator_ = SparseMatrix2D{rows, columns};
    for (auto const & item : interpolation_op) {
        auto fine_variable = item.first.first;
        auto coarse_variable = item.first.second;
        auto value = item.second;
        interpolation_operator_(fine_variable, coarse_variable) = value;
    }
    interpolation_operator_.finalize();
//    interpolation_operator_.print();
}

void 
AMGStandardInterpolationPolicy::ComputeRestrictionOperator(SparseMatrix2D const & interpolation_operator) {
    restriction_operator_ = helper::transpose(interpolation_operator);
}

void 
AMGStandardInterpolationPolicy::ComputeGalerkinOperator(SparseMatrix2D const & m, SparseMatrix2D const & interpolation_operator, SparseMatrix2D const & restriction_operator) {
    galerkinOperator_ = helper::matrixMul(restriction_operator, helper::matrixMul(m, interpolation_operator));
    //Matrix2D m1 = helper::SparseToDense(m);
    //Matrix2D m2 = helper::SparseToDense(interpolation_operator);
    //Matrix2D m3 = helper::SparseToDense(restriction_operator);
    //galerkinOperator_ = helper::DenseToSparse(helper::matrixMul(m3, helper::matrixMul(m1, m2)));
}

SparseMatrix2D
AMGStandardInterpolationPolicy::GalerkinOperator() const {
    return galerkinOperator_;
}

SparseMatrix2D
AMGStandardInterpolationPolicy::Restrictor() const {
    return restriction_operator_;
}

SparseMatrix2D
AMGStandardInterpolationPolicy::Interpolator() const {
    return interpolation_operator_;
}

VariableCategorizer const &
AMGStandardInterpolationPolicy::GetVariableCategorizer() const {
    return *variable_categorizer_;
}
