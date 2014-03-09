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


namespace LinearSolverLibrary_NS {

    template<typename AMGPolicy>
    class AMGSolver {


        friend class BasicAMGSolverTest_TestExpectedGridHierarchyDepth_Test;


    public:
        AMGSolver(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & b, IAMGCycle const & cycle_scheme, AMGMonitor & monitor)
            :
            m_{m},
            b_{b},
            cycle_scheme_{cycle_scheme},
            monitor_{monitor} {

            BuildGalerkinOperatorHierarchy();
            BuildGridHierarchy();
            LUDecompositionForLastLevelGalerkinOperator();
        }

        void
        BuildGridHierarchy() {
            cycle_scheme_.initialize(static_cast<short>(amg_levels_.size()));
        }

        void
        BuildGalerkinOperatorHierarchy() {
            AMGHierarchyBuilder<AMGPolicy> amg_builder{monitor_};
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
            size_type level = 0;
            size_type prev_level = -1;
            size_type max_level = amg_levels_.size() - 1;
            amg_levels_[0].x_ = x_initial;
            Vector f{b};

            short direction = 1;
            do {
                level = cycle_scheme.currentLevel();
                short direction = level - prev_level;
                auto & amg_level = amg_levels_[level];
                if (level == max_level) {
                    // solve directly via LU decomposition
                    amg_level.x_ = LUSolve(amg_level.A_, f);
                    prev_level = level;
                    cycle_scheme.setNextLevel();
                    continue;
                }
                x = amg_level.x_;
                if (direction > 0) {
                    // restrict, move to next coarser level
                    x = pre_smooth(amg_level.A_, f, x);
                    Vector r_h = f - amg_level.A_ * x;
                    Vector r_2h = amg_level.restrictor * r_h;
                    f = r_2h;
                    cycle_scheme.setNextLevel();
                    prev_level = level;
                    continue;
                }
                else {
                    // interpolate, i.e. move to next finer level
                    Vector e_h = amg_levels_[level - 1].interpolator * x;
                    x = amg_levels_[level - 1].x + e_h;
                    x = post_smooth(amg_level.A_, f, x);
                    cycle_scheme.setNextLevel();
                    prev_level = level;
                    continue;
                }
            } while (cycle_scheme.next());
            return x;
        }

        LinAlg_NS::Vector
        solveFMG(LinAlg_NS::Vector const & x_initial) const {
        }

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
        LinAlg_NS::Vector const &         b_;
        IAMGCycle const &                 cycle_scheme_;
        AMGMonitor &                      monitor_;
        std::vector<AMGLevel>             amg_levels_;
        LUDecomposition                   lu_;
    };
    
} // LinearSolverLibrary_NS
