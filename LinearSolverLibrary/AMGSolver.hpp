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

    template<typename AMGPolicy, typename AMGCycleScheme, typename AMGRelaxationPolicy = void>
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
            cycle_scheme_.build(static_cast<short>(amg_levels_.size()));
        }

        void
        BuildGalerkinOperatorHierarchy() {
            AMGHierarchyBuilder<AMGPolicy, AMGRelaxationPolicy> amg_builder{monitor_};
            amg_levels_ = amg_builder.build(m_);
        }

        void
        LUDecompositionForLastLevelGalerkinOperator() {
            auto const & last_level = amg_levels_[amg_levels_.size() - 1];
            Matrix2D dense = helper::SparseToDense(last_level.m);
            bool success = lu_.decompose(dense);
            common_NS::reporting::checkConditional(success, "AMGSolver::LUDecompositionForLastLevelGalerkinOperator: LU decomposition failed");
        }

        LinAlg_NS::Vector
        solve(LinAlg_NS::Vector const & x_initial) const {
            using size_type = IMatrix2D::size_type;
            amg_levels_[0].x = x_initial;
            Vector f{b_};
            Vector x{x_initial};

            auto cycle_scheme_it = std::begin(cycle_scheme_);
            auto cycle_scheme_end_it = std::end(cycle_scheme_);
            int grid_level{0};
            int prev_grid_level{0};
            int max_level{static_cast<int>(amg_levels_.size() - 1)};

            short direction{1};
            while (cycle_scheme_it != cycle_scheme_end_it) {
                grid_level = *cycle_scheme_it;
                short direction = grid_level - prev_grid_level;
                auto & amg_level = amg_levels_[grid_level];
                if (grid_level == max_level) {
                    // solve directly via LU decomposition
                    amg_level.x = lu_.solve(f);
                    prev_grid_level = grid_level;
                    ++cycle_scheme_it;
                    continue;
                }
                x = amg_level.x;
                if (direction > 0) {
                    // restrict, move to next coarser level
//                    x = pre_smooth(amg_level.m, f, x);
                    Vector r_h = f - amg_level.m * x;
                    Vector r_2h = (*amg_level.restrictor) * r_h;
                    f = r_2h;
                    prev_grid_level = grid_level;
                    ++cycle_scheme_it;
                    continue;
                }
                else {
                    // interpolate, i.e. move to next finer level
                    Vector e_h = *(amg_levels_[grid_level - 1].interpolator) * x;
                    x = amg_levels_[grid_level - 1].x + e_h;
//                    x = post_smooth(amg_level.m, f, x);
                    prev_grid_level = grid_level;
                    ++cycle_scheme_it;
                    continue;
                }
            }
            return x;
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
