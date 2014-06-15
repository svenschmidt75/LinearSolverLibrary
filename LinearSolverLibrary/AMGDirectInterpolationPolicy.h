/*
* Name  : AMGDirectInterpolationPolicy
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "AMGInterpolationPolicyBase.h"

#include "DeclSpec.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IAMGStandardStrengthPolicy;
    class VariableCategorizer;
}

class AMGDirectInterpolationPolicyTest_MadeUpExample_Test;


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGDirectInterpolationPolicy : public AMGInterpolationPolicyBase {

        
        friend class AMGDirectInterpolationPolicyTest_MadeUpExample_Test;


    private:
        bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, IAMGStandardStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer);
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
