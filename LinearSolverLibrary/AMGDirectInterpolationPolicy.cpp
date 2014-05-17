#include "pch.h"

#include "AMGDirectInterpolationPolicy.h"
#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableCategorizer.h"
#include "VariableInfluenceAccessor.h"
#include "AMGStandardSplitting.h"
#include "AMGCoarseVariableIndexer.h"

using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGDirectInterpolationPolicy::AMGDirectInterpolationPolicy() {}

bool
AMGDirectInterpolationPolicy::Generate(SparseMatrix2D const & m) {
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
AMGDirectInterpolationPolicy::ComputeInterpolationOperator(SparseMatrix2D const & m, AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) {
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
            double a_num = 0;
            double a_denum = 0;
            double b_num = 0;
            double b_denum = 0;
            double diag = 0;

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
            for (auto coarse_variable : *interpolatory_set) {
                if (variable_categorizer.GetType(coarse_variable) != VariableCategorizer::Type::COARSE)
                    continue;
                double a_ik = m(fine_variable, coarse_variable);
                if (a_ik < 0)
                    a_denum += a_ik;
                else
                    b_denum += a_ik;
            }

            auto column_iterator = *row_it;
            for (; column_iterator; ++column_iterator) {
                auto k = column_iterator.column();
                double a_ik = m(fine_variable, k);
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

            for (auto coarse_variable : *interpolatory_set) {
                if (variable_categorizer.GetType(coarse_variable) != VariableCategorizer::Type::COARSE)
                    continue;
                double w_ik;
                double a_ik = m(fine_variable, coarse_variable);
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
AMGDirectInterpolationPolicy::CreateInterpolationOperator(size_type rows, size_type columns, Interpolation_t const & interpolation_op) {
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
AMGDirectInterpolationPolicy::ComputeRestrictionOperator(SparseMatrix2D const & interpolation_operator) {
    restriction_operator_ = helper::transpose(interpolation_operator);
}

void 
AMGDirectInterpolationPolicy::ComputeGalerkinOperator(SparseMatrix2D const & m, SparseMatrix2D const & interpolation_operator, SparseMatrix2D const & restriction_operator) {
    galerkinOperator_ = helper::matrixMul(restriction_operator, helper::matrixMul(m, interpolation_operator));
    //Matrix2D m1 = helper::SparseToDense(m);
    //Matrix2D m2 = helper::SparseToDense(interpolation_operator);
    //Matrix2D m3 = helper::SparseToDense(restriction_operator);
    //galerkinOperator_ = helper::DenseToSparse(helper::matrixMul(m3, helper::matrixMul(m1, m2)));
}

SparseMatrix2D
AMGDirectInterpolationPolicy::GalerkinOperator() const {
    return galerkinOperator_;
}

SparseMatrix2D
AMGDirectInterpolationPolicy::Restrictor() const {
    return restriction_operator_;
}

SparseMatrix2D
AMGDirectInterpolationPolicy::Interpolator() const {
    return interpolation_operator_;
}

VariableCategorizer const &
AMGDirectInterpolationPolicy::GetVariableCategorizer() const {
    return *variable_categorizer_;
}
