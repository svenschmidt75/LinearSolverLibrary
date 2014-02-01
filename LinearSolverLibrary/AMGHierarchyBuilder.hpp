/*
* Name  : AMGSolverAMGHierarchyBuilder
* Path  :
* Use   : Generates AMG hierarchy
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "AMGLevel.h"


namespace LinearSolverLibrary_NS {

    template<typename AMGPolicy>
    class AMGSolverAMGHierarchyBuilder {
    public:
        AMGSolverAMGHierarchyBuilder(LinAlg_NS::SparseMatrix2D const & m)
            :
            m_{m},
            amg_policy_{AMGPolicy{}} {}

        std::vector<AMGLevel>
        build() const {
            std::vector<AMGLevel> amg_levels;

            AMGLevel level0;
            level0.m = m;
            level0.interpolator = amg_policy_.generateInterpolationOperator(m);
            level0.prolongator = transpose(level0.interpolator);

            return amg_levels;
        }

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
        AMGPolicy                         amg_policy_;
    };

} // LinearSolverLibrary_NS
