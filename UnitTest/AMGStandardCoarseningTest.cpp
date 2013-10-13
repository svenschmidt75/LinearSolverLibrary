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
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 1", strength_policy.VariableDependsOn(0, 1));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 3", strength_policy.VariableDependsOn(0, 3));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection on itself", !strength_policy.VariableDependsOn(0, 0));

    // row 3
    CPPUNIT_ASSERT_MESSAGE("Variable 3 should not have a strong connection to variable 0", strength_policy.VariableDependsOn(3, 0));
    CPPUNIT_ASSERT_MESSAGE("Variable 3 should not have a strong connection to variable 4", strength_policy.VariableDependsOn(3, 4));
    CPPUNIT_ASSERT_MESSAGE("Variable 3 should not have a strong connection to variable 6", strength_policy.VariableDependsOn(3, 6));
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
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 1", strength_policy.VariableDependsOn(0, 1));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 3", strength_policy.VariableDependsOn(0, 3));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection to variable 4", strength_policy.VariableDependsOn(0, 4));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not have a strong connection on itself", !strength_policy.VariableDependsOn(0, 0));

    // row 1
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 0", strength_policy.VariableDependsOn(1, 0));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 2", strength_policy.VariableDependsOn(1, 2));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 3", strength_policy.VariableDependsOn(1, 3));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 4", strength_policy.VariableDependsOn(1, 4));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection to variable 5", strength_policy.VariableDependsOn(1, 5));
    CPPUNIT_ASSERT_MESSAGE("Variable 1 should not have a strong connection on itself", !strength_policy.VariableDependsOn(1, 1));

    // row 4
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 0", strength_policy.VariableDependsOn(4, 0));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 1", strength_policy.VariableDependsOn(4, 1));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 2", strength_policy.VariableDependsOn(4, 2));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 3", strength_policy.VariableDependsOn(4, 3));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 5", strength_policy.VariableDependsOn(4, 5));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 6", strength_policy.VariableDependsOn(4, 6));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 7", strength_policy.VariableDependsOn(4, 7));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection to variable 8", strength_policy.VariableDependsOn(4, 8));
    CPPUNIT_ASSERT_MESSAGE("Variable 4 should not have a strong connection on itself", !strength_policy.VariableDependsOn(4, 4));

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmetric(m));
}
