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
AMGStandardCoarseningTest::Test1() {
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
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should depend on variable 1", strength_policy.VariableDependsOn(0, 1));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should depend on variable 3", strength_policy.VariableDependsOn(0, 3));
    CPPUNIT_ASSERT_MESSAGE("Variable 0 should not depend on itself", strength_policy.VariableDependsOn(0, 0));
}
