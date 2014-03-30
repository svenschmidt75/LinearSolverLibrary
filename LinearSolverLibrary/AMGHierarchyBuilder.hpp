/*
* Name  : AMGHierarchyBuilder
* Path  :
* Use   : Generates AMG hierarchy
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "AMGLevel.h"


namespace LinearSolverLibrary_NS {


    // forward declarations
    struct AMGMonitor;


    template<typename AMGPolicy, typename AMGRelaxationPolicy = void>
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

            // TODO SS: Inject IVariableAccessor or create adapter
            // that exposes the set of variables (coarse, fine, undefined)...
            AMGPolicy amg_policy;

            // TODO SS: Find better name
            int const max_size = monitor_.direct_solver_threshold_;


            // TODO SS: test this branch!!!
            if (m.cols() < max_size) {
                AMGLevel amg_level;
                amg_level.m = m;
                amg_levels_.emplace_back(amg_level);
            } else {
                // do 1st level
                Handle1stLevel(amg_policy, m);
                MoveToNextLevel();
                while (SolveWithDirectMethod(GetCurrentLevel()->m, max_size) == false) {
                    amg_levels_.emplace_back();
                    AMGLevel * current_level = GetCurrentLevel();
                    AMGLevel * next_level = GetNextLevel();
                    MoveToNextLevel();
                    amg_policy.Generate(current_level->m);
                    current_level->restrictor = std::make_shared<SparseMatrix2D>(amg_policy.Restrictor());
                    next_level->interpolator = std::make_shared<SparseMatrix2D>(amg_policy.Interpolator());
                    next_level->m = amg_policy.GalerkinOperator();
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
        Handle1stLevel(AMGPolicy & amg_policy, SparseMatrix2D const & m) const {
            amg_levels_.emplace_back();
            amg_levels_.emplace_back();
            AMGLevel * current_level = GetCurrentLevel();
            AMGLevel * next_level = GetNextLevel();
            amg_policy.Generate(m);
            current_level->m = m;
            current_level->restrictor = std::make_shared<SparseMatrix2D>(amg_policy.Restrictor());
            next_level->interpolator = std::make_shared<SparseMatrix2D>(amg_policy.Interpolator());
            next_level->m = amg_policy.GalerkinOperator();
        }

    private:
        AMGMonitor const &            monitor_;
        mutable std::vector<AMGLevel> amg_levels_;
        mutable int                   current_level_index_;
    };

} // LinearSolverLibrary_NS
