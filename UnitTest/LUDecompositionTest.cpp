#include "pch.h"

#include "LUDecompositionTest.h"

#include "LinearSolverLibrary/LUDecomposition.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
LUDecompositionTest::setUp() {}

void
LUDecompositionTest::tearDown() {}

namespace {

    bool almostEqual(double d1, double d2, double tol = 1E-3) {
        if (d1 == 0.0)
            return true;
        double diff = std::fabs((d1 - d2) / d1);
        bool success = diff < tol;
        return success;
    }
}

void
LUDecompositionTest::Test3by3_1() {
    // generated from http://comnuan.com/cmnn0100d/
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

//     A.print();

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
4.00000   6.00000   8.00000   
0.00000   -0.50000   -1.00000   
0.00000   0.00000   1.00000   


Matrix L:
1.00000   0.00000   0.00000   
0.25000   1.00000   0.00000   
0.50000   0.00000   1.00000   


Permutation P:
3   1   2   
*/
    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 0.25));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 0.5));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 0.0));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 6.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), -0.5));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 8.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), -1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 1.0));
}

void
LUDecompositionTest::Test3by3_2() {
    // Example from http://site.iugaza.edu.ps/iismail/files/Example-on-LU-decomposition-with-partial-pivoting1.pdf
    // generated from http://comnuan.com/cmnn0100d/
    Matrix2D A(3, 3);

    A(0, 0) = 2;
    A(0, 1) = -6;
    A(0, 2) = -1;

    A(1, 0) = -3;
    A(1, 1) = -1;
    A(1, 2) = 7;

    A(2, 0) = -8;
    A(2, 1) = 1;
    A(2, 2) = -2;

//     A.print();

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
-8.00000   1.00000   -2.00000   
0.00000   -5.75000   -1.50000   
0.00000   0.00000   8.10870   


Matrix L:
1.00000   0.00000   0.00000   
-0.25000   1.00000   0.00000   
0.37500   0.23913   1.00000   


Permutation P:
3   1   2   
*/
    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), -0.25));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 0.375));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 0.23913));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), -8.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), -5.75));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), -2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), -1.5));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 8.1087));
}

void
LUDecompositionTest::Test3by3_3() {
    // Example from http://www-old.math.gatech.edu/academic/courses/core/math2601/Web-notes/2num.pdf
    // generated from http://comnuan.com/cmnn0100d/
    Matrix2D A(3, 3);

    A(0, 0) = 1;
    A(0, 1) = 1;
    A(0, 2) = -2;

    A(1, 0) = 1;
    A(1, 1) = 3;
    A(1, 2) = -1;

    A(2, 0) = 1;
    A(2, 1) = 5;
    A(2, 2) = 1;

//     A.print();

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
1.00000   1.00000   -2.00000   
0.00000   4.00000   3.00000   
0.00000   0.00000   -0.50000   


Matrix L:
1.00000   0.00000   0.00000   
1.00000   1.00000   0.00000   
1.00000   0.50000   1.00000   


Permutation P:
1   3   2   */
    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 0.5));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), -2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), -0.5));
}

void
LUDecompositionTest::Test3by3_4() {
    // generated from http://comnuan.com/cmnn0100d/
    Matrix2D A(3, 3);

    A(0, 0) = 1;
    A(0, 1) = 1;
    A(0, 2) = 1;

    A(1, 0) = 2;
    A(1, 1) = 2;
    A(1, 2) = 5;

    A(2, 0) = 4;
    A(2, 1) = 6;
    A(2, 2) = 8;

//     A.print();

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

//     ludecomp.LU_->print();

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
4.00000   6.00000   8.00000   
0.00000   -1.00000   1.00000   
0.00000   0.00000   -1.50000   


Matrix L:
1.00000   0.00000   0.00000   
0.50000   1.00000   0.00000   
0.25000   0.50000   1.00000   


Permutation P:
3   2   1   
*/

    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 0.5));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 0.25));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 0.5));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 6.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), -1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 8.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), -1.5));


    // solve for rhs
    Vector rhs(3);
    rhs(0) = 1;
    rhs(1) = 0;
    rhs(2) = 0;
    Vector x = ludecomp.solve(rhs);

    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(0), 7.0 / 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(1), - 2.0 / 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(2), - 2.0 / 3.0));
}

void
LUDecompositionTest::Test3by3_5() {
    // Introduction to Algorithms, Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein
    // page 818
    // generated from http://comnuan.com/cmnn0100d/
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

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

//    ludecomp.LU_->print();

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
5.00000   6.00000   3.00000   
0.00000   0.80000   -0.60000   
0.00000   0.00000   2.50000   


Matrix L:
1.00000   0.00000   0.00000   
0.20000   1.00000   0.00000   
0.60000   0.50000   1.00000   


Permutation P:
3   1   2   
*/

    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 0.2));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 0.6));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 0.5));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 5.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 6.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), 0.8));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 3.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), -0.6));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 2.5));


    // solve for rhs
    Vector rhs(3);
    rhs(0) = 3;
    rhs(1) = 7;
    rhs(2) = 8;
    Vector x = ludecomp.solve(rhs);

    // generated from http://bmanolov.free.fr/solve_lineq.php

    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(0), - 7.0 / 5.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(1), 11.0 / 5.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(2), 3.0 / 5.0));
}

void
LUDecompositionTest::Test4by4_1() {
    // Introduction to Algorithms, Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein
    // page 822
    // generated from http://comnuan.com/cmnn0100d/
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

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
6.00000   13.00000   5.00000   19.00000   
0.00000   14.66667   8.33333   16.66667   
0.00000   0.00000   6.90909   16.81818   
0.00000   0.00000   0.00000   -0.03947   


Matrix L:
1.00000   0.00000   0.00000   0.00000   
0.33333   1.00000   0.00000   0.00000   
0.66667   0.09091   1.00000   0.00000   
0.33333   -0.09091   0.01316   1.00000   


Permutation P:
2   3   4   1   
*/
    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 0.33333));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 0.66667));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 0), 0.33333));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 0.09091));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 1), -0.09091));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 2), 0.01316));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 6.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 13.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), 14.66667));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 5.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 8.33333));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 6.90909));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 3), 19.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 3), 16.66667));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 3), 16.81818));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 3), -0.03947));
}

void
LUDecompositionTest::Test4by4_2() {
    // Trefethen, Bau, page 158
    // generated from http://comnuan.com/cmnn0100d/

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

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
8.00000   7.00000   9.00000   5.00000   
0.00000   1.75000   2.25000   4.25000   
0.00000   0.00000   -0.85714   -0.28571   
0.00000   0.00000   0.00000   0.66667   


Matrix L:
1.00000   0.00000   0.00000   0.00000   
0.75000   1.00000   0.00000   0.00000   
0.50000   -0.28571   1.00000   0.00000   
0.25000   -0.42857   0.33333   1.00000   


Permutation P:
3   4   2   1   
*/
    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 0.75));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 0.5));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 0), 0.25));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), -0.28571));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 1), -0.42857));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 2), 0.33333));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 8.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 7.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), 1.75));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 9.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 2.25));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), -0.85714));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 3), 5.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 3), 4.25));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 3), -0.28571));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 3), 0.66667));
}

void
LUDecompositionTest::Test4by4_3() {
    // Introduction to Algorithms, Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein
    // page 826
    // generated from http://comnuan.com/cmnn0100d/
    Matrix2D A(4, 4);

    A(0, 0) = 2;
    A(0, 1) = 0;
    A(0, 2) = 2;
    A(0, 3) = 0.6;

    A(1, 0) = 3;
    A(1, 1) = 3;
    A(1, 2) = 4;
    A(1, 3) = -2;

    A(2, 0) = 5;
    A(2, 1) = 5;
    A(2, 2) = 4;
    A(2, 3) = 2;

    A(3, 0) = -1;
    A(3, 1) = -2;
    A(3, 2) = 3.4;
    A(3, 3) = -1;

//     A.print();

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
5.00000   5.00000   4.00000   2.00000   
0.00000   -2.00000   0.40000   -0.20000   
0.00000   0.00000   4.00000   -0.50000   
0.00000   0.00000   0.00000   -3.00000   


Matrix L:
1.00000   0.00000   0.00000   0.00000   
0.40000   1.00000   0.00000   0.00000   
-0.20000   0.50000   1.00000   0.00000   
0.60000   0.00000   0.40000   1.00000   


Permutation P:
3   1   4   2   
*/
    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 0.4));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), -0.2));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 0), 0.6));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), 0.5));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 1), 0.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 2), 0.4));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), 5.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 5.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), -2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 0.4));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 4.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 3), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 3), -0.2));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 3), -0.5));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 3), -3.0));
}

void
LUDecompositionTest::Test4by4_4() {
    // Example from generated from http://comnuan.com/cmnn0100d/
    Matrix2D A(4, 4);

    A(0, 0) = -1;
    A(0, 1) = 9;
    A(0, 2) = 2;
    A(0, 3) = 3;

    A(1, 0) = 5;
    A(1, 1) = 6;
    A(1, 2) = -5;
    A(1, 3) = -2;

    A(2, 0) = -9;
    A(2, 1) = 0;
    A(2, 2) = 1;
    A(2, 3) = 9;

    A(3, 0) = 0;
    A(3, 1) = -3;
    A(3, 2) = 7;
    A(3, 3) = -4;

//     A.print();

    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(A);
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", success);

    auto const LU = ludecomp.rearrangeDueToPivoting();

//     LU.print();

/*
Matrix U:
-9.00000   0.00000   1.00000   9.00000   
0.00000   9.00000   1.88889   2.00000   
0.00000   0.00000   7.62963   -3.33333   
0.00000   0.00000   0.00000   -0.82524   


Matrix L:
1.00000   0.00000   0.00000   0.00000   
0.11111   1.00000   0.00000   0.00000   
0.00000   -0.33333   1.00000   0.00000   
-0.55556   0.66667   -0.74757   1.00000   


Permutation P:
3   1   4   2   
*/
    // check L part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 0), 0.11111));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 0), 0.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 0), -0.55556));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 1), -0.33333));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 1), 0.66667));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 2), -0.74757));

    // check U part
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 0), -9.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 1), 0.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 1), 9.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 2), 1.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 2), 1.88889));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 2), 7.62963));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(0, 3), 9.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(1, 3), 2.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(2, 3), -3.33333));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(LU(3, 3), -0.82524));


    // solve for rhs
    Vector rhs(4);
    rhs(0) = 3;
    rhs(1) = 7;
    rhs(2) = 8;
    rhs(3) = 1;
    Vector x = ludecomp.solve(rhs);

    // generated from http://bmanolov.free.fr/solve_lineq.php

    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(0), -259.0 / 17.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(1), 229.0 / 51.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(2), -98.0 / 17.0));
    CPPUNIT_ASSERT_MESSAGE("error in LU decomposition", almostEqual(x(3), -233.0 / 17.0));
}
