/*
* Name  : AMGHierarchyBuilder
* Path  :
* Use   : Generates AMG hierarchy
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "AMGLevel.h"
#include "AMGCThenFRelaxationPolicy.hpp"


namespace LinearSolverLibrary_NS {


    // forward declarations
    struct AMGMonitor;


    template<typename AMGInterpolationPolicy, typename AMGRelaxationPolicy = AMGCThenFRelaxationPolicy>
    class AMGHierarchyBuilder {
    public:
        AMGHierarchyBuilder(AMGMonitor const & monitor)
            :
            monitor_{monitor},
            current_level_index_{0} {}

        AMGHierarchyBuilder(AMGHierarchyBuilder const &) = delete;
        AMGHierarchyBuilder & operator=(AMGHierarchyBuilder const &) = delete;

        std::vector<AMGLevel>
        build(SparseMatrix2D const & m) const {
            AMGInterpolationPolicy interpolation_policy;
            AMGRelaxationPolicy relaxation_policy;

            int const direct_solver_threshold = monitor_.direct_solver_threshold;

            if (m.cols() < direct_solver_threshold) {
                throw std::logic_error("AMGHierarchyBuilder::build: The matrix has less columns than the direct solve threashold\n" \
                    "resulting in a 1-level AMG hierarchy");
            } else {
                Handle1stLevel(interpolation_policy, relaxation_policy, m);
                MoveToNextLevel();
                while (SolveWithDirectMethod(GetCurrentLevel()->m, direct_solver_threshold) == false) {
                    amg_levels_.emplace_back();
                    AMGLevel * current_level = GetCurrentLevel();
                    AMGLevel * next_level = GetNextLevel();
                    MoveToNextLevel();
                    interpolation_policy.Generate(current_level->m);
                    current_level->restrictor = std::make_shared<SparseMatrix2D>(interpolation_policy.Restrictor());
                    current_level->variableDecomposition = relaxation_policy.Decompose(interpolation_policy);
                    next_level->interpolator = std::make_shared<SparseMatrix2D>(interpolation_policy.Interpolator());
                    next_level->m = interpolation_policy.GalerkinOperator();
                }
            }
            return amg_levels_;
        }

        AMGLevel *
        GetCurrentLevel() const {
            return &amg_levels_[current_level_index_];
        }

        AMGLevel *
        GetNextLevel() const {
            return &amg_levels_[current_level_index_ + 1];
        }

        void
        MoveToNextLevel() const {
            current_level_index_++;
        }

        bool
        SolveWithDirectMethod(SparseMatrix2D const & m, int max_size) const {
            return m.cols() <= max_size;
        }

        void
        Handle1stLevel(AMGInterpolationPolicy & interpolation_policy, AMGRelaxationPolicy & relaxation_policy, SparseMatrix2D const & m) const {
            amg_levels_.emplace_back();
            amg_levels_.emplace_back();
            AMGLevel * current_level = GetCurrentLevel();
            AMGLevel * next_level = GetNextLevel();
            interpolation_policy.Generate(m);
            current_level->m = m;
            current_level->variableDecomposition = relaxation_policy.Decompose(interpolation_policy);
            current_level->restrictor = std::make_shared<SparseMatrix2D>(interpolation_policy.Restrictor());
            next_level->interpolator = std::make_shared<SparseMatrix2D>(interpolation_policy.Interpolator());
            next_level->m = interpolation_policy.GalerkinOperator();
        }

    private:
        AMGMonitor const &            monitor_;
        mutable std::vector<AMGLevel> amg_levels_;
        mutable int                   current_level_index_;
    };

} // LinearSolverLibrary_NS
