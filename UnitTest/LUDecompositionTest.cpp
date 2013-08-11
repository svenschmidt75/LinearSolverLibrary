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

namespace {

    bool almostEqual(double d1, double d2, double tol = 1E-2) {
        double diff = std::fabs(d1 - d2) / d1;
        bool success = diff < tol;
        return success;
    }
}

void
LUDecompositionTest::Test3by3_1() {
    Matrix2D A(3, 3);

    A(0, 0) = 1;
    A(0, 1) = 1;
    A(0, 2) = 1;

    A(1, 0) = 2;
    A(1, 1) = 3;
    A(1, 2) = 5;

    A(2, 0) = 4;
    A(2, 1) = 6;
    A(2, 2) = 8;

    A.print();

    LUDecomposition ludecomp = LinearSolverLibrary_NS::LUDecomposition();
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    Matrix2D const & LU = *(ludecomp.LU_);

    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 2.0));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), -2.0));

}
