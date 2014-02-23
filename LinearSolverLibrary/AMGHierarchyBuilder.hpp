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


    template<typename AMGPolicy>
    class AMGHierarchyBuilder {
    public:
        AMGHierarchyBuilder(AMGMonitor const & monitor) : monitor_{monitor} {}
        AMGHierarchyBuilder(AMGHierarchyBuilder const &) = delete;
        AMGHierarchyBuilder & operator=(AMGHierarchyBuilder const &) = delete;

        std::vector<AMGLevel>
        build(SparseMatrix2D const & m) const {
            AMGPolicy amg_policy;
            std::vector<AMGLevel> amg_levels;

            int const max_size = monitor_.min_galerkin_size_;




            // TODO SS: test this branch!!!
            if (m.cols() < max_size) {
                AMGLevel amg_level;
                amg_level.m = m;
                amg_levels.emplace_back(amg_level);
            }
            else {
                // do 1st level
                amg_levels.emplace_back();
                amg_levels.emplace_back();
                int current_level_index = 0;
                int next_level_index = current_level_index + 1;

                AMGLevel * current_level = &amg_levels[current_level_index];
                AMGLevel * next_level = &amg_levels[next_level_index];

                amg_policy.generate(m);
                current_level->m = m;
                current_level->restrictor = std::make_shared<SparseMatrix2D>(amg_policy.Restrictor());
                next_level->interpolator = std::make_shared<SparseMatrix2D>(amg_policy.Interpolator());
                next_level->m = amg_policy.GalerkinMatrix();

                current_level = next_level;
                current_level_index = next_level_index;

                // all further levels

                while (current_level->m.cols() > max_size) {
                    amg_levels.emplace_back();
                    current_level = &amg_levels[current_level_index++];
                    next_level_index++;
                    next_level = &amg_levels[next_level_index];
                    auto const & A = current_level->m;
                    amg_policy.generate(A);
                    current_level->restrictor = std::make_shared<SparseMatrix2D>(amg_policy.Restrictor());
                    next_level->interpolator = std::make_shared<SparseMatrix2D>(amg_policy.Interpolator());
                    next_level->m = amg_policy.GalerkinMatrix();
                    current_level = next_level;
                }
            }
            return amg_levels;
        }

    private:
        AMGMonitor const & monitor_;
    };

} // LinearSolverLibrary_NS
