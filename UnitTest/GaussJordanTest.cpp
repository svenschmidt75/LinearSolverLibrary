#include "pch.h"

#include "GaussJordanTest.h"

#include "LinearSolverLibrary/GaussJordan.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"

#include "LinAlg/entities.h"
#include "LinAlg/operators.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
GaussJordanTest::setUp() {}

void
GaussJordanTest::tearDown() {}

void
GaussJordanTest::Test2by2_1() {
    Matrix2D A(2, 2);

    A(0, 0) = 4;
    A(0, 1) = 3;

    A(1, 0) = 3;
    A(1, 1) = 2;

//     A.print();

    // generate r.h.s.
    Vector rhs(2);
    rhs(0) = 1;
    rhs(1) = 1;

    Matrix2D AInverse(2, 2);
    Vector x(2);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(2);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test2by2_2() {
    Matrix2D A(2, 2);

    A(0, 0) = 1E-20;
    A(0, 1) = 1;

    A(1, 0) = 1;
    A(1, 1) = 1;

//     A.print();

    // generate r.h.s.
    Vector rhs(2);
    rhs(0) = 1;
    rhs(1) = 1;

    Matrix2D AInverse(2, 2);
    Vector x(2);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(2);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test3by3_1() {
    Matrix2D A(3, 3);

    A(0, 0) = 2;
    A(0, 1) = -1;
    A(0, 2) = 0;

    A(1, 0) = -1;
    A(1, 1) = 2;
    A(1, 2) = -1;

    A(2, 0) = 0;
    A(2, 1) = -1;
    A(2, 2) = 2;

//     A.print();

    // generate r.h.s.
    Vector rhs(3);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;

    Matrix2D AInverse(3, 3);
    Vector x(3);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(3);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test3by3_2() {
    Matrix2D A(3, 3);

    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;

    A(1, 0) = 0;
    A(1, 1) = 1;
    A(1, 2) = 4;

    A(2, 0) = 5;
    A(2, 1) = 6;
    A(2, 2) = 0;

//     A.print();

    // generate r.h.s.
    Vector rhs(3);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;

    Matrix2D AInverse(3, 3);
    Vector x(3);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(3);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test3by3_3() {
    Matrix2D A(3, 3);

    A(0, 0) = 1;
    A(0, 1) = 3;
    A(0, 2) = 3;

    A(1, 0) = 1;
    A(1, 1) = 4;
    A(1, 2) = 3;

    A(2, 0) = 1;
    A(2, 1) = 3;
    A(2, 2) = 4;

//     A.print();

    // generate r.h.s.
    Vector rhs(3);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;

    Matrix2D AInverse(3, 3);
    Vector x(3);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(3);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test3by3_4() {
    Matrix2D A(3, 3);

    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 0;

    A(1, 0) = 3;
    A(1, 1) = 4;
    A(1, 2) = 4;

    A(2, 0) = 5;
    A(2, 1) = 6;
    A(2, 2) = 3;

//     A.print();

    // generate r.h.s.
    Vector rhs(3);
    rhs(0) = 3;
    rhs(1) = 7;
    rhs(2) = 8;

    Matrix2D AInverse(3, 3);
    Vector x(3);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(3);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test4by4_1() {
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

//     A.print();

    // generate r.h.s.
    Vector rhs(4);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;
    rhs(3) = 1;

    Matrix2D AInverse(4, 4);
    Vector x(4);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(4);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test4by4_2() {
    Matrix2D A(4, 4);

    A(0, 0) = 1;
    A(0, 1) = -1;
    A(0, 2) = 0;
    A(0, 3) = 0;

    A(1, 0) = -1;
    A(1, 1) = 2;
    A(1, 2) = -1;
    A(1, 3) = 0;

    A(2, 0) = 0;
    A(2, 1) = -1;
    A(2, 2) = 2;
    A(2, 3) = -1;

    A(3, 0) = 0;
    A(3, 1) = 0;
    A(3, 2) = -1;
    A(3, 3) = 2;

//     A.print();

    // generate r.h.s.
    Vector rhs(4);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;
    rhs(3) = 1;

    Matrix2D AInverse(4, 4);
    Vector x(4);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(4);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test4by4_3() {
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

//     A.print();

    // generate r.h.s.
    Vector rhs(4);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;
    rhs(3) = 1;

    Matrix2D AInverse(4, 4);
    Vector x(4);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);


    Vector x_ref(4);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test4by4_4() {
    Matrix2D A(4, 4);

    A(0, 0) = 4;
    A(0, 1) = 8;
    A(0, 2) = 4;
    A(0, 3) = 0;

    A(1, 0) = 1;
    A(1, 1) = 4;
    A(1, 2) = 7;
    A(1, 3) = 2;

    A(2, 0) = 1;
    A(2, 1) = 5;
    A(2, 2) = 4;
    A(2, 3) = -3;

    A(3, 0) = 1;
    A(3, 1) = 3;
    A(3, 2) = 0;
    A(3, 3) = -2;

//     A.print();

    // generate r.h.s.
    Vector rhs(4);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;
    rhs(3) = 1;

    Matrix2D AInverse(4, 4);
    Vector x(4);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);

    Vector x_ref(4);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
GaussJordanTest::Test5by5_1() {
    Matrix2D A(5, 5);

    A(0, 0) = 1000;
    A(0, 1) = 4;
    A(0, 2) = 6;
    A(0, 3) = 8;
    A(0, 4) = 10;

    A(1, 0) = 4;
    A(1, 1) = 1100;
    A(1, 2) = 16;
    A(1, 3) = 18;
    A(1, 4) = 20;

    A(2, 0) = 6;
    A(2, 1) = 16;
    A(2, 2) = 1200;
    A(2, 3) = 28;
    A(2, 4) = 30;

    A(3, 0) = 8;
    A(3, 1) = 18;
    A(3, 2) = 28;
    A(3, 3) = 1300;
    A(3, 4) = 40;

    A(4, 0) = 10;
    A(4, 1) = 20;
    A(4, 2) = 30;
    A(4, 3) = 40;
    A(4, 4) = 1400;

    //     A.print();

    // generate r.h.s.
    Vector rhs(5);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;
    rhs(3) = 1;
    rhs(4) = 1;

    Matrix2D AInverse(5, 5);
    Vector x(4);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::GaussJordan().solve(A, rhs);

    Vector x_ref(5);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}
