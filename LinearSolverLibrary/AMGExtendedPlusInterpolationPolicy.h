/*
 * Name  : AMGExtendedPlusInterpolationPolicy
 * Path  :
 * Use   :
 * Author: Sven Schmidt
 * Date  : 06/14/2014
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

GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTest, TestStrongFFConnection);
GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTest, TestStrongFFConnectionWithNoCommonCNode);
GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTest, TestStrongFFConnectionWithCommonCNode);
GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithWeakConnections);
GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithPositiveOffDiagonalEntries);
GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithStrongPositiveOffDiagonalEntries);


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGExtendedPlusInterpolationPolicy : public AMGInterpolationPolicyBase {


        FRIEND_GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTest, TestStrongFFConnection);
        FRIEND_GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTest, TestStrongFFConnectionWithNoCommonCNode);
        FRIEND_GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTest, TestStrongFFConnectionWithCommonCNode);
        FRIEND_GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithWeakConnections);
        FRIEND_GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithPositiveOffDiagonalEntries);
        FRIEND_GOOGLE_TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithStrongPositiveOffDiagonalEntries);
        friend class ComplexGraphTest_NS::ComplexGraphTest<AMGExtendedPlusInterpolationPolicy>;

    private:
        bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, IAMGStandardStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) override;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
