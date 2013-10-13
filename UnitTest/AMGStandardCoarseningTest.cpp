#include "pch.h"

#include "AMGStandardCoarseningTest.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.h"
#include "LinAlg/helper.h"

#include "LinearSolverLibrary/AMGStandardCoarseningStrengthPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGStandardCoarseningTest::setUp() {}

void
AMGStandardCoarseningTest::tearDown() {}

void
AMGStandardCoarseningTest::TestStrongConnectionsForRowForFivePointStencil() {
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

    // row 0
    auto variable_set = strength_policy.GetInfluencedByVariables(0);
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 1", variable_set.contains(1));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 3", variable_set.contains(3));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection on itself", variable_set.size() == 2);

    // row 3
    variable_set = strength_policy.GetInfluencedByVariables(3);
    CPPUNIT_ASSERT_MESSAGE("Variable 3 should not have a strong connection to variable 0", variable_set.contains(0));
    CPPUNIT_ASSERT_MESSAGE("Variable 3 should not have a strong connection to variable 4", variable_set.contains(4));
    CPPUNIT_ASSERT_MESSAGE("Variable 3 should not have a strong connection to variable 6", variable_set.contains(6));
    CPPUNIT_ASSERT_MESSAGE("Variable 3 should not have a strong connection on itself", variable_set.size() == 3);
}

void
AMGStandardCoarseningTest::TestStrongConnectionsForRowForNinePointStencil() {
    MatrixStencil stencil;
    stencil << -1, -4,  -1,
               -4, 20,  -4,
               -1, -4,  -1;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();
/*
      20       -4        0       -4       -1        0        0        0        0
      -4       20       -4       -1       -4       -1        0        0        0
       0       -4       20        0       -1       -4        0        0        0
      -4       -1        0       20       -4        0       -4       -1        0
      -1       -4       -1       -4       20       -4       -1       -4       -1
       0       -1       -4        0       -4       20        0       -1       -4
       0        0        0       -4       -1        0       20       -4        0
       0        0        0       -1       -4       -1       -4       20       -4
       0        0        0        0       -1       -4        0       -4       20
*/

    AMGStandardCoarseningStrengthPolicy strength_policy(m);

    // row 0
    auto variable_set = strength_policy.GetInfluencedByVariables(0);
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 1", variable_set.contains(1));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 3", variable_set.contains(3));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 4", variable_set.contains(4));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection on itself", variable_set.size() == 3);

    // row 1
    variable_set = strength_policy.GetInfluencedByVariables(1);
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 0", variable_set.contains(0));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 2", variable_set.contains(2));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 3", variable_set.contains(3));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 4", variable_set.contains(4));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 5", variable_set.contains(5));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection on itself", variable_set.size() == 5);

    // row 4
    variable_set = strength_policy.GetInfluencedByVariables(4);
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 0", variable_set.contains(0));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 1", variable_set.contains(1));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 2", variable_set.contains(2));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 3", variable_set.contains(3));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 5", variable_set.contains(5));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 6", variable_set.contains(6));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 7", variable_set.contains(7));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 8", variable_set.contains(8));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection on itself", variable_set.size() == 8);

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmetric(m));
}
