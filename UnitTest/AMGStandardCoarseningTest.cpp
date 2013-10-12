#include "pch.h"

#include "AMGStandardCoarseningTest.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.h"

#include "LinearSolverLibrary/AMGStandardCoarseningStrengthPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGStandardCoarseningTest::setUp() {}

void
AMGStandardCoarseningTest::tearDown() {}

void
AMGStandardCoarseningTest::TestStrongConnectionsForRow() {
    MatrixStencil stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    Matrix2D reference_matrix(9, 9);
    reference_matrix <<
        4, -1, 0, -1, 0, 0, 0, 0, 0,
        -1, 4, -1, 0, -1, 0, 0, 0, 0,
        0, -1, 4, 0, 0, -1, 0, 0, 0,
        -1, 0, 0, 4, -1, 0, -1, 0, 0,
        0, -1, 0, -1, 4, -1, 0, -1, 0,
        0, 0, -1, 0, -1, 4, 0, 0, -1,
        0, 0, 0, -1, 0, 0, 4, -1, 0,
        0, 0, 0, 0, -1, 0, -1, 4, -1,
        0, 0, 0, 0, 0, -1, 0, -1, 4;

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
