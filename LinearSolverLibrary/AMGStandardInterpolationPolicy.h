/*
* Name  : AMGStandardInterpolationPolicy
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 05/18/2014
*/
#pragma once

#include "AMGInterpolationPolicyBase.h"

#include "DeclSpec.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class AMGStandardStrengthPolicy;
    class VariableCategorizer;
}

class AMGStandardInterpolationPolicyTest_TestStrongFFConnection_Test;


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardInterpolationPolicy : public AMGInterpolationPolicyBase {


        friend class AMGStandardInterpolationPolicyTest_TestStrongFFConnection_Test;


    private:
        bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, AMGStandardStrengthPolicy const & strength_policy, IVariableCategorizer const & variable_categorizer);
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
