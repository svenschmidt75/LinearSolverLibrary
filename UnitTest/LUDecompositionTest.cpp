#include "pch.h"

#include "LUDecompositionTest.h"

#include "LinearSolverLibrary/LUDecomposition.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"

#include "LinAlg/entities.h"
#include "LinAlg/operators.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
LUDecompositionTest::setUp() {}

void
LUDecompositionTest::tearDown() {}

void
LUDecompositionTest::Test3by3_1() {
    Matrix2D A(3, 3);

    A(0, 0) = 1;
    A(0, 1) = 1;
    A(0, 2) = 1;

    A(1, 0) = 2;
    A(1, 1) = 3;
    A(1, 2) = 6;

    A(2, 0) = 4;
    A(2, 1) = 6;
    A(2, 2) = 8;

    A.print();

    bool success = LinearSolverLibrary_NS::LUDecomposition().decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);
}
