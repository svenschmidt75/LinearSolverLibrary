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
        AMGStandardSplittingPolicy(LinAlg_NS::SparseMatrix2D const & m);

        AMGStandardSplittingPolicy(AMGStandardSplittingPolicy const &) = delete;
        AMGStandardSplittingPolicy & operator=(AMGStandardSplittingPolicy const &) = delete;

    private:
        LinAlg_NS::SparseMatrix2D const &                    m_;
        std::unique_ptr<AMGStandardCoarseningStrengthPolicy> strength_policy_;
        std::unique_ptr<VariableCategorizer>                 variable_categorizer_;
        std::unique_ptr<VariableInfluenceAccessor>           variable_influence_accessor_;
        std::unique_ptr<AMGStandardSplitting>                splitting_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
