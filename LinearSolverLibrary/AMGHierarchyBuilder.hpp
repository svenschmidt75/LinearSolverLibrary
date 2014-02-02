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

    template<typename AMGPolicy>
    class AMGHierarchyBuilder {
    public:
        AMGHierarchyBuilder(LinAlg_NS::SparseMatrix2D const & m)
            :
            m_{m},
            amg_policy_{AMGPolicy{}} {}

        std::vector<AMGLevel>
        build() const {
            std::vector<AMGLevel> amg_levels;
            int const max_size = 100;
            if (m.cols() < max_size) {
                AMGLevel amg_level;
                amg_level.m = std::make_unique<SparseMatrix2D>(m);
                amg_levels.push_back(amg_level);
            }
            else {
                // do 1st level
                amg_levels.push_back(AMGLevel{});
                amg_levels.push_back(AMGLevel{});
                int current_level_index = 0;
                int next_level_index = current_level_index + 1;

                AMGLevel * current_level = &amg_levels[current_level_index];
                AMGLevel * next_level = &amg_levels[current_level_index];

                amg_policy_.generate(m);
                current_level.m = std::make_unique<SparseMatrix2D>(m);
                current_level.prolongator = amg_policy_.prolongator(m);
                next_level.interpolator = amg_policy_.interpolator();
                next_level.m = amg_policy_.coarseA();

                current_level = next_level;
                current_level_index = next_level_index;


                while (current_level.m.cols() > max_size) {
                    amg_levels.push_back(AMGLevel{});
                    next_level_index++;
                    next_level = &amg_levels[next_level_index];
                    auto const & A = current_level->m;
                    current_level.m = std::make_unique<SparseMatrix2D>(A);
                    amg_policy_.generate(A);
                    current_level.prolongator = amg_policy_.prolongator();
                    next_level.interpolator = amg_policy_.interpolator();
                    current_level = next_level;
                }

                // do LU decomposition for last level
            }
            return amg_levels;
        }

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
        AMGPolicy                         amg_policy_;
    };

} // LinearSolverLibrary_NS
