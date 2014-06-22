#include "pch.h"

#include "AMGExtendedPlusInterpolationPolicy.h"
#include "IAMGStandardStrengthPolicy.h"
#include "VariableCategorizer.h"
#include "AMGCoarseVariableIndexer.h"
#include "IVariableSet.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


bool
AMGExtendedPlusInterpolationPolicy::ComputeInterpolationOperator(SparseMatrix2D const & m, IAMGStandardStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) {
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
                // Fine variable 'fine_variable' has no strong connections to any other coarse variable.
                // This means that it can be solved for directly as it has no dependencies on any other
                // variables. We therefore stop coarsening at this level and solve via direct solve (LU).
                // Alternatively, the fine variable could be removed from the system. Then, we continue
                // coarsening with the reduced system.
                //                m.print();
                return false;
            }

            using size_type = IMatrix2D::size_type;

            
            // contains those F variables that strongly influence i
            // F_{i}^{s}
            std::set<size_type> strong_fine_influencer;

            // Find the set of variables in N_{i} that is weakly connected to variable i
            // by taking the complement of N_{i} and S_{i}.
            std::set<size_type> weakly_connected_variable_set;
            auto column_iterator = *row_it;
            for (; column_iterator; ++column_iterator) {
                auto j = column_iterator.column();
                if (j == fine_variable)
                    continue;
                if (interpolatory_set->contains(j)) {
                    // j strongly influences i
                    if (variable_categorizer.GetType(j) == VariableCategorizer::Type::FINE)
                        strong_fine_influencer.insert(j);
                }
                else
                    // j does not strongly influence i, so the connection i-j is weak
                    weakly_connected_variable_set.insert(j);
            }


            // Creates the extended set of interpolatory points \hat{C}_{i}.
            // It contains all strong coarse i-j connections. If j is fine,
            // we add j=s strong coarse variables as well.
            // We also add i itself.
            std::set<size_type> extended_coarse_variable_set_plus;

            for (auto j : *interpolatory_set) {
                // i-j connection of type F-F?
                if (variable_categorizer.GetType(j) != VariableCategorizer::Type::FINE) {
                    // no, i-j is F-C
                    if (variable_categorizer.GetType(j) == VariableCategorizer::Type::COARSE)
                        extended_coarse_variable_set_plus.insert(j);
                    continue;
                }

                // i-j connection of type F-F
                auto j_interpolatory_set = strength_policy.GetInfluencedByVariables(j);
                for (auto k : *j_interpolatory_set) {
                    if (variable_categorizer.GetType(k) == VariableCategorizer::Type::COARSE)
                        extended_coarse_variable_set_plus.insert(k);
                }
            }

            std::set<size_type> extended_coarse_variable_set{extended_coarse_variable_set_plus};
            extended_coarse_variable_set_plus.insert(fine_variable);


            // compute the sum over the weakly influencing variables
            double weak_sum = 0;
            for (auto j : weakly_connected_variable_set) {
                if (m(fine_variable, j)) {
                    double a_ij = m(fine_variable, j);
                    weak_sum += a_ij;
                }
            }



            double const a_ii = m(fine_variable, fine_variable);



            // compute the sum over the extended coarse set
            using size_type = IMatrix2D::size_type;
            std::map<size_type, double> extended_sums;
            for (auto k : strong_fine_influencer) {
                double extended_sum = 0;
                for (auto l : extended_coarse_variable_set_plus) {
                    double a_kl = m(k, l);
                    if (a_kl * a_ii < 0)
                        extended_sum += a_kl;
                }

                if (extended_sum == 0)
                    throw std::runtime_error("AMGExtendedPlusInterpolationPolicy::ComputeInterpolationOperator: Division by zero");

                extended_sums[k] = extended_sum;
            }



            // compute the sum over F_{i}^{s} in \tilde{a}_{ii}, the modified diagonal value
            double a_ii_hat = m(fine_variable, fine_variable);
            a_ii_hat += weak_sum;
            for (auto k : strong_fine_influencer) {
                double a_ki = m(k, fine_variable);
                if (a_ki * a_ii < 0) {
                    double a_ik = m(fine_variable, k);

                    BOOST_ASSERT_MSG(extended_sums.find(k) != std::end(extended_sums), "AMGExtendedPlusInterpolationPolicy::ComputeInterpolationOperator: Couldn't find extended sum for coarse variable");

                    double value = a_ik * a_ki / extended_sums[k];
                    a_ii_hat += value;
                }
            }



            for (auto j : extended_coarse_variable_set) {
                if (variable_categorizer.GetType(j) != VariableCategorizer::Type::COARSE)
                    continue;

                double w_ik = m(fine_variable, j);

                for (auto k : strong_fine_influencer) {
                    double a_kj = m(k, j);
                    if (a_kj * a_ii < 0) {
                        double a_ik = m(fine_variable, k);
                        double value = a_ik * a_kj;

                        BOOST_ASSERT_MSG(extended_sums.find(k) != std::end(extended_sums), "AMGExtendedPlusInterpolationPolicy::ComputeInterpolationOperator: Couldn't find extended sum for coarse variable");

                        value /= extended_sums[k];
                        w_ik += value;
                    }
                }

                if (w_ik == 0)
                    continue;

                w_ik /= - a_ii_hat;

                auto cv = indexer.Index(j);
                interpolation_op[{fine_variable, cv}] = w_ik;
            }
        }
        ++row_it;
    }
    CreateInterpolationOperator(m.rows(), indexer.NumberOfVariables(), interpolation_op);
    return true;
}
