/*
* Name  : AMGDirectInterpolation
* Path  :
* Use   : Compute prolongation/restriction operator as
          described in Trottenberg, page 479, A.7.2.1
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once


#include "VariableCategorizer.h"

#include "LinAlg/SparseMatrix2D.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    template<typename AMGStandardCoarseningPolicy>
    class AMGDirectInterpolation {
    public:
        AMGDirectInterpolation(LinAlg_NS::SparseMatrix2D const & m) {
            AMGStandardCoarseningStrengthPolicy strength_policy(m);
            VariableCategorizer variable_categorizer(m.rows());
            VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
            AMGStandardSplitting splitting(m, influence_accessor, variable_categorizer);
            splitting.generateSplitting();
        }

        AMGDirectInterpolation(AMGDirectInterpolation const &) = delete;
        AMGDirectInterpolation & operator=(AMGDirectInterpolation const &) = delete;

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
        VariableCategorizer &             categorizer_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
