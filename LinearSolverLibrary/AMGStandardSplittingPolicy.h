/*
* Name  : AMGStandardSplittingPolicy
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


// forward declarations
namespace LinearSolverLibrary_NS {
    class AMGStandardCoarseningStrengthPolicy;
    class VariableCategorizer;
    class VariableInfluenceAccessor;
    class AMGStandardSplitting;
}

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardSplittingPolicy {
    public:
        AMGStandardSplittingPolicy();

        AMGStandardSplittingPolicy(AMGStandardSplittingPolicy const &) = delete;
        AMGStandardSplittingPolicy & operator=(AMGStandardSplittingPolicy const &) = delete;

        bool                      generate(LinAlg_NS::SparseMatrix2D const & m);
        LinAlg_NS::SparseMatrix2D prolongator() const;
        LinAlg_NS::SparseMatrix2D interpolator() const;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
