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
    class IAMGStandardStrengthPolicy;
    class VariableCategorizer;
}

class AMGStandardInterpolationPolicyTest_TestStrongFFConnection_Test;
class AMGStandardInterpolationPolicyTest_TestStrongFFConnectionWithMock_Test;
class AMGStandardInterpolationPolicyTest_TestComplexGraph_Test;
class AMGStandardInterpolationPolicyTest_TestStrongFFConnectionWithNoCommonCNode_Test;
class AMGStandardInterpolationPolicyTest_TestStrongFFConnectionWithCommonCNode_Test;


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardInterpolationPolicy : public AMGInterpolationPolicyBase {


        friend class AMGStandardInterpolationPolicyTest_TestStrongFFConnection_Test;
        friend class AMGStandardInterpolationPolicyTest_TestStrongFFConnectionWithMock_Test;
        friend class AMGStandardInterpolationPolicyTest_TestComplexGraph_Test;
        friend class AMGStandardInterpolationPolicyTest_TestStrongFFConnectionWithNoCommonCNode_Test;
        friend class AMGStandardInterpolationPolicyTest_TestStrongFFConnectionWithCommonCNode_Test;

    private:
        bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, IAMGStandardStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) override;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
