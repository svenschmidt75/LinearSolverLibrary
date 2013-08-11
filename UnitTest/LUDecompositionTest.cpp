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

void
LUDecompositionTest::Test4by4_1() {
    Matrix2D A(4, 4);

    A(0, 0) = 2;
    A(0, 1) = 3;
    A(0, 2) = 1;
    A(0, 3) = 5;

    A(1, 0) = 6;
    A(1, 1) = 13;
    A(1, 2) = 5;
    A(1, 3) = 19;

    A(2, 0) = 2;
    A(2, 1) = 19;
    A(2, 2) = 10;
    A(2, 3) = 23;

    A(3, 0) = 4;
    A(3, 1) = 10;
    A(3, 2) = 11;
    A(3, 3) = 31;

    A.print();

    LUDecomposition ludecomp = LinearSolverLibrary_NS::LUDecomposition();
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    Matrix2D const & LU = *(ludecomp.LU_);

    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 0), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 2), 7.0));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 3), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 3), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 3), 3.0));
}

void
LUDecompositionTest::Test4by4_2() {
    Matrix2D A(4, 4);

    A(0, 0) = 2;
    A(0, 1) = 1;
    A(0, 2) = 1;
    A(0, 3) = 0;

    A(1, 0) = 4;
    A(1, 1) = 3;
    A(1, 2) = 3;
    A(1, 3) = 1;

    A(2, 0) = 8;
    A(2, 1) = 7;
    A(2, 2) = 9;
    A(2, 3) = 5;

    A(3, 0) = 6;
    A(3, 1) = 7;
    A(3, 2) = 9;
    A(3, 3) = 8;

    A.print();

    LUDecomposition ludecomp = LinearSolverLibrary_NS::LUDecomposition();
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    Matrix2D const & LU = *(ludecomp.LU_);

    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 0), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 2), 7.0));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 3), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 3), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 3), 3.0));
}
