#include "pch.h"

#include "VariableInfluenceAccessorTest.h"
#include "LinearSolverLibrary/AMGStandardCoarseningStrengthPolicy.h"
#include "LinearSolverLibrary/VariableInfluenceAccessor.h"
#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
VariableInfluenceAccessorTest::setUp() {}

void
VariableInfluenceAccessorTest::tearDown() {}

void
VariableInfluenceAccessorTest::TestFineGridVariablesInfluencedByCornerGridVariableWhenAllUndefined() {
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
VariableInfluenceAccessorTest::TestUndefinedVariablesInfluencedByCornerGridVariableAllUndefined() {
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
VariableInfluenceAccessorTest::TestVariablesInfluencedByCenterVariable() {
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

void
VariableInfluenceAccessorTest::TestFineGridVariablesInfluencedByBoundaryCenterVariableWhenAllUndefined() {
    MatrixStencil stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//     m.print();
/*
    8       -1        0       -1       -1        0        0        0        0
   -1        8       -1       -1       -1       -1        0        0        0
    0       -1        8        0       -1       -1        0        0        0
   -1       -1        0        8       -1        0       -1       -1        0
   -1       -1       -1       -1        8       -1       -1       -1       -1
    0       -1       -1        0       -1        8        0       -1       -1
    0        0        0       -1       -1        0        8       -1        0
    0        0        0       -1       -1       -1       -1        8       -1
    0        0        0        0       -1       -1        0       -1        8
*/

    AMGStandardCoarseningStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);

    // S_{5}^{T}: The set of variables strongly influenced by variable 5
    // S_{5}^{T} = {1, 2, 4, 7, 8}
    auto undefinedVariablesInfluencedBy = influence_accessor.GetVariableInfluencedUndefined(5);
    auto expected = undefinedVariablesInfluencedBy->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 5 should not influence any fine grid variables", expected, 5ull);
    CPPUNIT_ASSERT_MESSAGE("Variable 5 should influence undefined variable 1", undefinedVariablesInfluencedBy->contains(1));
    CPPUNIT_ASSERT_MESSAGE("Variable 5 should influence undefined variable 2", undefinedVariablesInfluencedBy->contains(2));
    CPPUNIT_ASSERT_MESSAGE("Variable 5 should influence undefined variable 4", undefinedVariablesInfluencedBy->contains(4));
    CPPUNIT_ASSERT_MESSAGE("Variable 5 should influence undefined variable 7", undefinedVariablesInfluencedBy->contains(7));
    CPPUNIT_ASSERT_MESSAGE("Variable 5 should influence undefined variable 8", undefinedVariablesInfluencedBy->contains(8));
}

void
VariableInfluenceAccessorTest::TestInfluencedByCenterVariable() {
    MatrixStencil stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//     m.print();
/*
    8       -1        0       -1       -1        0        0        0        0
   -1        8       -1       -1       -1       -1        0        0        0
    0       -1        8        0       -1       -1        0        0        0
   -1       -1        0        8       -1        0       -1       -1        0
   -1       -1       -1       -1        8       -1       -1       -1       -1
    0       -1       -1        0       -1        8        0       -1       -1
    0        0        0       -1       -1        0        8       -1        0
    0        0        0       -1       -1       -1       -1        8       -1
    0        0        0        0       -1       -1        0       -1        8
*/

    AMGStandardCoarseningStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);

    variable_categorizer.SetType(0, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(6, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(5, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(8, VariableCategorizer::Type::COARSE);

    // S_{4}^{T}: The set of variables strongly influenced by variable 4
    // S_{4}^{T} = {0, 1, 2, 3, 5, 6, 7, 8}
    auto undefinedVariablesInfluencedBy = influence_accessor.GetVariableInfluencedUndefined(4);
    auto expected = undefinedVariablesInfluencedBy->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 5 should influence 2 undefined variables", expected, 2ull);
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should influence undefined variable 2", undefinedVariablesInfluencedBy->contains(2));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should influence undefined variable 7", undefinedVariablesInfluencedBy->contains(7));

    auto fineVariablesInfluencedBy = influence_accessor.GetVariableInfluencedFine(4);
    expected = fineVariablesInfluencedBy->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Variable 5 should influence 3 fine grid variables", expected, 3ull);
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should influence undefined variable 0", fineVariablesInfluencedBy->contains(0));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should influence undefined variable 1", fineVariablesInfluencedBy->contains(1));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should influence undefined variable 6", fineVariablesInfluencedBy->contains(6));
}
