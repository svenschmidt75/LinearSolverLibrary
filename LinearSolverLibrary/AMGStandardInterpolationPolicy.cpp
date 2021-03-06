#include "pch.h"

#include "AMGStandardInterpolationPolicy.h"
#include "IAMGStrengthPolicy.h"
#include "VariableCategorizer.h"
#include "AMGCoarseVariableIndexer.h"
#include "IVariableSet.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


bool
AMGStandardInterpolationPolicy::ComputeInterpolationOperator(SparseMatrix2D const & m, IAMGStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) {
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
            auto interpolatory_set = strength_policy.getStrongInfluencers(fine_variable);
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

            // generate \hat{a}_{ij} set
            std::map<size_type, double> a_i_hat;

            std::set<size_type> extended_neighborhood_set;


            /*****************************************************
             * extract the a_ik from i's direct neighborhood N_i *
             *****************************************************/

            auto column_iterator = *row_it;
            for (; column_iterator; ++column_iterator) {
                auto k = column_iterator.column();
                if (k != fine_variable && variable_categorizer.GetType(k) == VariableCategorizer::Type::FINE) {
                    // Ignore all of i's (fine_variable) strong F-F neighbors for now, but
                    // do include weak F-F connections.
                    if (interpolatory_set->contains(k))
                        continue;
                }
                double a_ik = m(fine_variable, k);
                a_i_hat[k] = a_ik;
                extended_neighborhood_set.insert(k);
            }


             // Note: extended_neighborhood_set also contains fine_variable, although
             // i is not in N_{i}.


            // coarse set of i and of all its strong F-F connections
            std::set<size_type> extended_coarse_variable_set;


            /***************************************************************************
             * Replace all e_j where j is a strong fine variable, with its coarse a_jk *
             ***************************************************************************/


            for (auto j : *interpolatory_set) {
                if (variable_categorizer.GetType(j) != VariableCategorizer::Type::FINE) {
                    if (variable_categorizer.GetType(j) == VariableCategorizer::Type::COARSE)
                        extended_coarse_variable_set.insert(j);
                    continue;
                }

                // j is a fine variable

                auto j_interpolatory_set = strength_policy.getStrongInfluencers(j);
                for (auto k : *j_interpolatory_set) {
                    if (variable_categorizer.GetType(k) == VariableCategorizer::Type::COARSE) {
                        // fine_variable - j is a strong F-F connections. If k is a common coarse
                        // variable, add it so it is used in interpolation to fine_variable.
                        bool add_variable = true;
                        if (m(fine_variable, k)) {
                            // only add if strong
                            if (interpolatory_set->contains(k) == false) {
                                // fine_variable - k is a weak F-C connection.
                                // We still add it if the strong F-F connection fine_variable - j
                                // have no common coarse nodes.
                                std::vector<size_type> common_interpolation_points;
                                std::set_intersection(
                                    std::cbegin(*interpolatory_set),
                                    std::cend(*interpolatory_set),
                                    std::cbegin(*j_interpolatory_set),
                                    std::cend(*j_interpolatory_set),
                                    std::back_insert_iterator<std::vector<size_type>>(common_interpolation_points));

                                if (common_interpolation_points.empty() == false)
                                    // The strong F-F connection fine_variable - j have other common interpolatory
                                    // variables, hence do not add coarse variable k to interpolate to fine_variable
                                    // as fine_variable - k is a weak F-C connection.
                                    add_variable = false;
                            }
                        }
                        if (add_variable)
                            extended_coarse_variable_set.insert(k);
                    }
                }


                /***********************************
                 * generation of the \hat_{a}_{ik} *
                 ***********************************/

                double a_jj = m(j, j);
                double a_ij = m(fine_variable, j);
                double fac = - a_ij / a_jj;

                ConstRowColumnIterator<SparseMatrix2D> j_row_it = MatrixIterators::getConstRowColumnIterator(m, j);
                auto j_column_iterator = *j_row_it;
                for (; j_column_iterator; ++j_column_iterator) {
                    auto k = j_column_iterator.column();
                    if (k == j)
                        // skip a_jj
                        continue;
                    double a_jk = m(j, k);
                    a_i_hat[k] += fac * a_jk;
                    extended_neighborhood_set.insert(k);
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

            BOOST_ASSERT_MSG(a_i_hat.size() == extended_neighborhood_set.size(), "AMGStandardInterpolationPolicy::ComputeInterpolationOperator: Sizes should be equal");

            for (auto k : extended_neighborhood_set) {
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

                if (w_ik == 0)
                    continue;

                auto cv = indexer.Index(coarse_variable);
                interpolation_op[{fine_variable, cv}] = w_ik;
            }
        }
        ++row_it;
    }
    CreateInterpolationOperator(m.rows(), indexer.NumberOfVariables(), interpolation_op);
    return true;
}
