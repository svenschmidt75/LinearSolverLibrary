/*
* Name  : AMGDirectInterpolationPolicy
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "AMGInterpolationPolicyBase.h"

#include "..\UnitTest/UnitTestForwardDeclarationHelper.h"

#include "DeclSpec.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IAMGStrengthPolicy;
    class VariableCategorizer;
}

GOOGLE_TEST(AMGDirectInterpolationPolicyTest, MadeUpExample);
GOOGLE_TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithWeakConnections);
GOOGLE_TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithPositiveOffDiagonalEntries);
GOOGLE_TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithStrongPositiveOffDiagonalEntries);


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {


    class DECL_SYMBOLS AMGDirectInterpolationPolicy : public AMGInterpolationPolicyBase {

        
        FRIEND_GOOGLE_TEST(AMGDirectInterpolationPolicyTest, MadeUpExample);
        FRIEND_GOOGLE_TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithWeakConnections);
        FRIEND_GOOGLE_TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithPositiveOffDiagonalEntries);
        FRIEND_GOOGLE_TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithStrongPositiveOffDiagonalEntries);


    private:
        bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, IAMGStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) override;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
