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

        Vector
        Relax(AMGLevel const & amg_level, Vector const & x_initial, short max_iterations) const {
            bool success;
            int iterations;
            Vector x{x_initial.size()};
            std::tie(success, x, iterations) = SparseLinearSolverLibrary::SparseGSMultiColor(amg_level.m, x_initial, amg_level.f, amg_level.variableDecomposition, max_iterations);
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

        LinAlg_NS::Vector
        Solve(LinAlg_NS::Vector const & x_initial) const {
            Vector x{x_initial};
            double tolerance = 1E-16;
            int iteration = 0;
            int maxIterations = 100;
            while (iteration <= maxIterations) {
                x = Solve_internal(x);
                ++iteration;

                double normr = VectorMath::norm(b_ - m_ * x);
                double residual = normr / VectorMath::norm(b_);
                if (residual <= tolerance)
//                    return std::make_tuple(true, x, iteration, residual);
                    return x;
            }

            return x;
        }
        
        LinAlg_NS::Vector
        Solve_internal(LinAlg_NS::Vector const & x_initial) const {
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
        solveFMG(LinAlg_NS::Vector const & x_initial) const {
        }

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
        LinAlg_NS::Vector const &         b_;
        AMGCycleScheme                    cycle_scheme_;
        AMGMonitor &                      monitor_;
        mutable std::vector<AMGLevel>     amg_levels_;
        LUDecomposition                   lu_;
    };
    
} // LinearSolverLibrary_NS
