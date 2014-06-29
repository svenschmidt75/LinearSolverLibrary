/*
 * Name  : AMGExtendedPlusInterpolationPolicy
 * Path  :
 * Use   :
 * Author: Sven Schmidt
 * Date  : 06/14/2014
 */
#pragma once

#include "AMGInterpolationPolicyBase.h"

#include "DeclSpec.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IAMGStandardStrengthPolicy;
    class VariableCategorizer;
}

class AMGExtendedPlusInterpolationPolicyTest_TestStrongFFConnection_Test;
class AMGExtendedPlusInterpolationPolicyTest_TestComplexGraph_Test;
class AMGExtendedPlusInterpolationPolicyTest_TestStrongFFConnectionWithNoCommonCNode_Test;
class AMGExtendedPlusInterpolationPolicyTest_TestStrongFFConnectionWithCommonCNode_Test;
class AMGExtendedPlusInterpolationPolicyTestFromMGTutorial_MadeUpExampleWithWeakConnections_Test;
class AMGExtendedPlusInterpolationPolicyTestFromMGTutorial_MadeUpExampleWithPositiveOffDiagonalEntries_Test;
class AMGExtendedPlusInterpolationPolicyTestFromMGTutorial_MadeUpExampleWithStrongPositiveOffDiagonalEntries_Test;


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGExtendedPlusInterpolationPolicy : public AMGInterpolationPolicyBase {


        friend class AMGExtendedPlusInterpolationPolicyTest_TestStrongFFConnection_Test;
        friend class AMGExtendedPlusInterpolationPolicyTest_TestComplexGraph_Test;
        friend class AMGExtendedPlusInterpolationPolicyTest_TestStrongFFConnectionWithNoCommonCNode_Test;
        friend class AMGExtendedPlusInterpolationPolicyTest_TestStrongFFConnectionWithCommonCNode_Test;
        friend class AMGExtendedPlusInterpolationPolicyTestFromMGTutorial_MadeUpExampleWithWeakConnections_Test;
        friend class AMGExtendedPlusInterpolationPolicyTestFromMGTutorial_MadeUpExampleWithPositiveOffDiagonalEntries_Test;
        friend class AMGExtendedPlusInterpolationPolicyTestFromMGTutorial_MadeUpExampleWithStrongPositiveOffDiagonalEntries_Test;


    private:
        bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, IAMGStandardStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer);
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
