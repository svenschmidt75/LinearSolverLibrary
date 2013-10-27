#include "pch.h"

#include "VariableInfluenceAccessorTest.h"
#include "LinearSolverLibrary/AMGStandardCoarseningStrengthPolicy.h"
#include "LinearSolverLibrary/VariableInfluenceAccessor.h"
#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
VariableInfluenceAccessorTest::setUp() {}

void
VariableInfluenceAccessorTest::tearDown() {}

void
VariableInfluenceAccessorTest::TestFineGridVariablesInfluencedByVariable0WhenAllUndefined() {
    MatrixStencil stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();
/*
      4       -1        0       -1        0        0        0        0        0
     -1        4       -1        0       -1        0        0        0        0
      0       -1        4        0        0       -1        0        0        0
     -1        0        0        4       -1        0       -1        0        0
      0       -1        0       -1        4       -1        0       -1        0
      0        0       -1        0       -1        4        0        0       -1
      0        0        0       -1        0        0        4       -1        0
      0        0        0        0       -1        0       -1        4       -1
      0        0        0        0        0       -1        0       -1        4
*/

    AMGStandardCoarseningStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);

    // S_{0}^{T}: The set of variables strongly influenced by variable 0
    // S_{0}^{T} = {1, 3}
    auto fineGridVariablesInfluencedBy = influence_accessor.GetVariableInfluencedFine(0);
    auto it = std::begin(*fineGridVariablesInfluencedBy);
    auto expected = fineGridVariablesInfluencedBy->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("There should be none as all are undefined", expected, 0ull);
}

void
VariableInfluenceAccessorTest::TestUndefinedVariablesInfluencedByVariable0AllUndefined() {
    MatrixStencil stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();
/*
      4       -1        0       -1        0        0        0        0        0
     -1        4       -1        0       -1        0        0        0        0
      0       -1        4        0        0       -1        0        0        0
     -1        0        0        4       -1        0       -1        0        0
      0       -1        0       -1        4       -1        0       -1        0
      0        0       -1        0       -1        4        0        0       -1
      0        0        0       -1        0        0        4       -1        0
      0        0        0        0       -1        0       -1        4       -1
      0        0        0        0        0       -1        0       -1        4
*/

    AMGStandardCoarseningStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);

    // S_{0}^{T}: The set of variables strongly influenced by variable 0
    // S_{0}^{T} = {1, 3}
    auto fineGridVariablesInfluencedBy = influence_accessor.GetVariableInfluencedUndefined(0);
    auto it = std::begin(*fineGridVariablesInfluencedBy);
    decltype(*it) expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should influence this variable", expected, *it);
    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should influence this variable", expected, *++it);
    expected = fineGridVariablesInfluencedBy->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should influence this variable", expected, 2ull);
}
