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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should not influence any fine grid variables", expected, 0ull);
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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should influence variable 1", expected, *it);
    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should influence variable 3", expected, *++it);
    expected = fineGridVariablesInfluencedBy->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should only influence 2 (undefined) variables", expected, 2ull);
}

void
VariableInfluenceAccessorTest::TestVariablesInfluencedByVariable4() {
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

    // S_{4}^{T} = {1, 3, 5, 7}, the set of variables strongly influenced by variable 4
    // because:
    // S_{1} = {0, 2, 4}
    // S_{3} = {0, 4, 6}
    // S_{5} = {2, 4, 8}
    // S_{7} = {4, 6, 8}

    variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::FINE);

    auto fineGridVariablesInfluencedBy = influence_accessor.GetVariableInfluencedFine(4);
    auto it = std::begin(*fineGridVariablesInfluencedBy);
    decltype(*it) expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 4 should influence variable 1", expected, *it);
    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 4 should influence variable 3", expected, *++it);
    expected = fineGridVariablesInfluencedBy->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should only influence 2 fine grid variables", expected, 2ull);

    auto undefinedGridVariablesInfluencedBy = influence_accessor.GetVariableInfluencedUndefined(4);
    it = std::begin(*undefinedGridVariablesInfluencedBy);
    expected = 5;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 4 should influence variable 5", expected, *it);
    expected = 7;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 4 should influence variable 7", expected, *++it);
    expected = fineGridVariablesInfluencedBy->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 0 should only influence 2 undefined variables", expected, 2ull);
}
