/*
 * Name  : AMGStandardInterpolationPolicy
 * Path  :
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 05/18/2014
 */
#pragma once

#include "AMGInterpolationPolicyBase.h"

#include "..\UnitTest/UnitTestForwardDeclarationHelper.h"

#include "DeclSpec.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IAMGStandardStrengthPolicy;
    class VariableCategorizer;
}

namespace ComplexGraphTest_NS {
    template<typename INTERPOLATION_POLICY>
    class ComplexGraphTest;
}

GOOGLE_TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnection);
GOOGLE_TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithMock);
GOOGLE_TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithNoCommonCNode);
GOOGLE_TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithCommonCNode);


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardInterpolationPolicy : public AMGInterpolationPolicyBase {


        FRIEND_GOOGLE_TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnection);
        FRIEND_GOOGLE_TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithMock);
        FRIEND_GOOGLE_TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithNoCommonCNode);
        FRIEND_GOOGLE_TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithCommonCNode);
        friend class ComplexGraphTest_NS::ComplexGraphTest<AMGStandardInterpolationPolicy>;


    private:
        bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, IAMGStandardStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) override;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
