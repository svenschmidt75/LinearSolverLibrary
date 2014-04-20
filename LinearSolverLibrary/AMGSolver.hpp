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

        LinAlg_NS::Vector
        solve(LinAlg_NS::Vector const & x_initial) const {
            using size_type = IMatrix2D::size_type;

            Vector x{x_initial};

            auto cycle_scheme_it = std::begin(cycle_scheme_);
            auto cycle_scheme_end_it = std::end(cycle_scheme_);
            int grid_level;
            int prev_grid_level;
            int max_level{static_cast<int>(amg_levels_.size() - 1)};

            // handle 1st level
            auto & amg_level = amg_levels_[0];
            amg_level.x = x_initial;
            amg_level.f = b_;
            bool success;
            int iterations;
            std::tie(success, x, iterations) = SparseLinearSolverLibrary::SparseGSMultiColor(amg_level.m, amg_level.x, amg_level.f, amg_level.variableDecomposition, 1);
            amg_level.x = x;

            Vector r_h = amg_level.f - amg_level.m * x;
            amg_levels_[1].f = *(amg_level.restrictor) * r_h;

            prev_grid_level = 0;
            ++cycle_scheme_it;






            while (cycle_scheme_it != cycle_scheme_end_it) {
                grid_level = *cycle_scheme_it;
                short direction = grid_level - prev_grid_level;
                auto & amg_level = amg_levels_[grid_level];
                if (grid_level == max_level) {
                    // solve directly via LU decomposition
                    amg_level.x = lu_.solve(amg_level.f);
                    prev_grid_level = grid_level;
                    ++cycle_scheme_it;
                    continue;
                }
                if (direction > 0) {
                    // restrict, move to next coarser level
                    x = Vector{amg_level.m.cols()};
                    bool success;
                    int iterations;
                    std::tie(success, x, iterations) = SparseLinearSolverLibrary::SparseGSMultiColor(amg_level.m, x, amg_level.f, amg_level.variableDecomposition, 1);
                    amg_level.x = x;

                    Vector r_h = amg_level.f - amg_level.m * x;
                    amg_levels_[grid_level + 1].f = *(amg_level.restrictor) * r_h;

                    prev_grid_level = grid_level;
                    ++cycle_scheme_it;
                    continue;
                }
                else {
                    // interpolate, i.e. move to next finer level
                    Vector e_h = *(amg_levels_[grid_level + 1].interpolator) * amg_levels_[grid_level + 1].x;
                    amg_level.x += e_h;
                    bool success;
                    Vector new_x;
                    int iterations;
                    std::tie(success, x, iterations) = SparseLinearSolverLibrary::SparseGSMultiColor(amg_level.m, amg_level.x, amg_level.f, amg_level.variableDecomposition, 1);
                    amg_level.x = x;

                    prev_grid_level = grid_level;
                    ++cycle_scheme_it;
                    continue;
                }
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
