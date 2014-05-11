/*
* Name  : AMGSolver
* Path  :
* Use   : Generates AMG hierarchy
* Author: Sven Schmidt
* Date  : 01/18/2014
*/
#pragma once

#include "IAMGCycle.h"
#include "AMGLevel.h"
#include "AMGMonitor.h"
#include "AMGHierarchyBuilder.hpp"


// forward declarations
class BasicAMGSolverTest_TestExpectedGridHierarchyDepth_Test;
class BasicAMGSolverTest_TestExpectedVCycleDepth_Test;


namespace LinearSolverLibrary_NS {

    template<typename AMGInterpolationPolicy, typename AMGCycleScheme, typename AMGRelaxationPolicy = AMGCThenFRelaxationPolicy>
    class AMGSolver {


        friend class BasicAMGSolverTest_TestExpectedGridHierarchyDepth_Test;
        friend class BasicAMGSolverTest_TestExpectedVCycleDepth_Test;


    public:
        AMGSolver(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & b, AMGMonitor & monitor)
            :
            m_{m},
            b_{b},
            monitor_{monitor} {

            BuildGalerkinOperatorHierarchy();
            ComputeGridComplexity();
            ComputeOperatorComplexity();
            BuildGridHierarchy();
            LUDecompositionForLastLevelGalerkinOperator();
        }

        void
        BuildGridHierarchy() {
            cycle_scheme_.build(static_cast<short>(amg_levels_.size() - 1));
        }

        void
        BuildGalerkinOperatorHierarchy() {
            AMGHierarchyBuilder<AMGInterpolationPolicy, AMGRelaxationPolicy> amg_builder{monitor_};
            amg_levels_ = amg_builder.build(m_);
        }

        void
        LUDecompositionForLastLevelGalerkinOperator() {
            auto const & last_level = amg_levels_[amg_levels_.size() - 1];
            Matrix2D dense = helper::SparseToDense(last_level.m);
            bool success = lu_.decompose(dense);
            common_NS::reporting::checkConditional(success, "AMGSolver::LUDecompositionForLastLevelGalerkinOperator: LU decomposition failed");
        }

        void
        ComputeGridComplexity() const {
            BOOST_ASSERT(amg_levels_.size() > 0);
            auto fine_grid_row_count = amg_levels_[0].m.rows();
            float row_count{0};
            for (unsigned short i = 0; i < amg_levels_.size(); ++i) {
                auto rows = amg_levels_[i].m.rows();
                row_count += rows;
            }
            BOOST_ASSERT(row_count > 0);
            grid_complexity_ = row_count / fine_grid_row_count;
        }

        float
        OperatorComplexity() const {
            return operator_complexity_;
        }

        void
        ComputeOperatorComplexity() const {
            BOOST_ASSERT(amg_levels_.size() > 0);
            auto fine_grid_nnz = amg_levels_[0].m.nnz();
            float nnz_count{0};
            for (unsigned short i = 0; i < amg_levels_.size(); ++i) {
                auto nnz = amg_levels_[i].m.nnz();
                nnz_count += nnz;
            }
            BOOST_ASSERT(nnz_count > 0);
            operator_complexity_ = nnz_count / fine_grid_nnz;
        }

        float
        GridComplexity() const {
            return grid_complexity_;
        }

        Vector
        Relax(AMGLevel const & amg_level, Vector const & x_initial, short max_iterations) const {
            bool success;
            Vector x{x_initial.size()};
            std::tie(success, x) = AMGRelaxationPolicy::Solve(amg_level.m, x_initial, amg_level.f, amg_level.variableDecomposition, max_iterations, monitor_);
            return x;
        }

        void
        MoveDown(Vector const & x, int current_grid_level) const {
            auto & amg_level = amg_levels_[current_grid_level];
            amg_level.x = Relax(amg_level, x, monitor_.nu1);
            Vector r_h = amg_level.f - amg_level.m * amg_level.x;
            amg_levels_[current_grid_level + 1].f = *(amg_level.restrictor) * r_h;
        }

        void
        MoveUp(int current_grid_level) const {
            AMGLevel & amg_level = amg_levels_[current_grid_level];
            AMGLevel const & next_amg_level = amg_levels_[current_grid_level + 1];
            Vector e_h = *(next_amg_level.interpolator) * next_amg_level.x;
            amg_level.x += e_h;
            amg_level.x = Relax(amg_level, amg_level.x, monitor_.nu2);
        }

        void
        SolveExact(int current_grid_level) const {
            auto & amg_level = amg_levels_[current_grid_level];
            amg_level.x = lu_.solve(amg_level.f);
        }

        std::tuple<bool, LinAlg_NS::Vector>
        Solve(LinAlg_NS::Vector const & x_initial) const {
            Vector x{x_initial};
            double required_tolerance = monitor_.required_tolerance;
            double & achieved_tolerance = monitor_.achieved_tolerance;
            int & iteration = monitor_.iterations;
            iteration = 0;
            auto maxIterations = monitor_.nmax_iterations;
            double normb = VectorMath::norm(b_);
            double stall_previous_residual_norm = 1.0;
            double & normr = monitor_.residual;
            double previous_normr = 1.0;

            if (monitor_.verbosity)
                std::cout << "---- i ----     ---- r2 ----      ---- dr ----  " << std::endl;

            while (iteration <= maxIterations) {
                x = AMGFullCycle(x);
                ++iteration;

                normr = VectorMath::norm(b_ - m_ * x);

                /* The numerical scheme might get stuck, i.e. no improvements
                 * will be made w.r.t. the residual. In this case, there is
                 * no point in keeping the iteration running. This might happen
                 * for matrices with very high condition number.
                 */
                if (!(iteration % monitor_.nstall_iterations)) {
//                    double tmp = std::fabs(std::log2(normr / stall_previous_residual_norm));
                    double tmp = std::fabs(std::log2(normr) - std::log2(stall_previous_residual_norm));

                    // TODO SS: 0.07?
                    if (tmp < 0.08)
                        return std::make_tuple(true, x);
                    stall_previous_residual_norm = normr;
                }

                double residual_ratio = normr / previous_normr;
                previous_normr = normr;

                if (monitor_.verbosity)
                    std::cout << iteration << "      " << normr << "       " << residual_ratio << std::endl;

                achieved_tolerance = normr / normb;
                if (achieved_tolerance <= required_tolerance)
                    return std::make_tuple(true, x);
            }

            return std::make_tuple(false, x);
        }
        
        LinAlg_NS::Vector
        AMGFullCycle(LinAlg_NS::Vector const & x_initial) const {
            using size_type = IMatrix2D::size_type;

            auto cycle_scheme_it = std::begin(cycle_scheme_);
            auto cycle_scheme_end_it = std::end(cycle_scheme_);
            int grid_level{0};
            int prev_grid_level;
            int max_level{static_cast<int>(amg_levels_.size() - 1)};

            // handle 1st level
            auto & amg_level = amg_levels_[grid_level];
            amg_level.x = x_initial;
            amg_level.f = b_;
            MoveDown(x_initial, grid_level);

            prev_grid_level = 0;
            ++cycle_scheme_it;

            while (cycle_scheme_it != cycle_scheme_end_it) {
                grid_level = *cycle_scheme_it;
                short direction = grid_level - prev_grid_level;

                if (grid_level == max_level) {
                    // At the deepest level, we solve exact via LU decomposition.
                    SolveExact(grid_level);
                }
                else if (direction > 0) {
                    // restrict, move to next coarser level
                    auto & amg_level = amg_levels_[grid_level];
                    MoveDown(Vector::MakeZeroVector(amg_level.m.cols()), grid_level);
                } else {
                    // interpolate, i.e. move to next finer level
                    MoveUp(grid_level);
                }

                prev_grid_level = grid_level;
                ++cycle_scheme_it;
                continue;
            }
            return amg_levels_[0].x;
        }

        LinAlg_NS::Vector
        SolveFMG(LinAlg_NS::Vector const & x_initial) const {
        }

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
        LinAlg_NS::Vector const &         b_;
        AMGCycleScheme                    cycle_scheme_;
        AMGMonitor &                      monitor_;
        mutable std::vector<AMGLevel>     amg_levels_;
        LUDecomposition                   lu_;
        mutable float                     grid_complexity_;
        mutable float                     operator_complexity_;
    };
    
} // LinearSolverLibrary_NS
