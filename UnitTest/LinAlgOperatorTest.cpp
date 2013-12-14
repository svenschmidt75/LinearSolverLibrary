#include "pch.h"

#include "LinAlgOperatorTest.h"

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/Matrix2D.h"
#include "LinAlg/EntityOperators.h"
#include "LinAlg/MatrixStencil.hpp"
#include "LinAlg/PeriodicBoundaryConditionPolicy.hpp"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"

#include <numeric>


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
LinAlgOperatorTest::SparseMatrixRowIteratorTest() {}

void
LinAlgOperatorTest::tearDown() {}

namespace {

    Vector createVector(int size) {
        Vector v(size);
        return v;
    }

    Matrix2D createMatrix2D() {
        Matrix2D m(3, 2);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(1, 0) = 3;
        m(1, 1) = 4;
        m(2, 0) = 5;
        m(2, 1) = 6;
        return m;
    }

    SparseMatrix2D createSparseMatrix2D() {
        SparseMatrix2D m(3);
        m(0, 0) = 1;
        m(1, 1) = 3;
        m(1, 2) = 2;
        m(2, 2) = 6;
        m.finalize();
        return m;
    }

}

void
LinAlgOperatorTest::vectorMulScalarTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector result(v1.size());

    double scalar = 0.93847;
    result = scalar * v1;

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v1(i) * scalar;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }

    result = scalar * v1;

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v1(i) * scalar;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorAddScalarMulVectorTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector v2 = createVector(10);
    std::iota(std::begin(v2), std::end(v2), 20);

    Vector result(v1.size());

    double scalar = 0.93847;

    result = v1 + (scalar * v2);

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v1(i) + scalar * v2(i);

        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorMulEqualScalarTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector result{v1};

    double scalar = 0.93847;
    result *= scalar;

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v1(i) * scalar;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorAddVectorTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector v2 = createVector(10);
    std::iota(std::begin(v2), std::end(v2), 20);

    Vector result(v1.size());

    result = v1 + v2;

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v1(i) + v2(i);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorSubVectorTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector v2 = createVector(10);
    std::iota(std::begin(v2), std::end(v2), 20);

    Vector result(v1.size());

    result = v1 - v2;

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v1(i) - v2(i);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorExprMulScalarTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector v2 = createVector(10);
    std::iota(std::begin(v2), std::end(v2), 20);

    Vector result(v1.size());

    double scalar = 0.93847;
    result = scalar * (v1 + v2);

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = (v1(i) + v2(i)) * scalar;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorPlusEqualVectorTest() {
    Vector v1 = createVector(10);
    Vector v2 = createVector(10);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 5);

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v2(i) + v1(i);
        result(i) = val;
    }

    v2 += v1;

    for (int i = 0; i < 10; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", result(i), v2(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorPlusEqualVectorScalarExprTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector result{v1};

    double scalar = 0.93847;
    result += (scalar * v1);

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v1(i) + (v1(i) * scalar);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorPlusEqualVectorMulTest() {
    Vector v1 = createVector(10);
    Vector v2 = createVector(10);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 67);

    v2 += (10.0 * v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 77, v2(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 132, v2(5), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 176, v2(9), 1E-10);
}

void
LinAlgOperatorTest::addScalarMulVectorTest() {
    Vector v1 = createVector(10);
    Vector v2 = createVector(10);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 5);

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v2(i) + 0.1 * v1(i);
        result(i) = val;
    }

    v2 += 0.1 * v1;

    for (int i = 0; i < 10; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", result(i), v2(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorPlusEqualVectorBinaryExprTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector v2 = createVector(10);
    std::iota(std::begin(v2), std::end(v2), 20);

    Vector result{ v1 };
    result += (v1 - v2);

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v1(i) + (v1(i) - v2(i));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorMinusEqualVectorTest() {
    Vector v1 = createVector(10);
    Vector v2 = createVector(10);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 5);

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = v2(i) - v1(i);
        result(i) = val;
    }

    v2 -= v1;

    for (int i = 0; i < 10; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", result(i), v2(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorExprDivScalarTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector v2 = createVector(10);
    std::iota(std::begin(v2), std::end(v2), 20);

    Vector result(v1.size());

    double scalar = 0.93847;
    result = (v1 + v2) / scalar;

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = (v1(i) + v2(i)) / scalar;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorExprAddVectorExprTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector v2 = createVector(10);
    std::iota(std::begin(v2), std::end(v2), 20);

    Vector result(v1.size());
    result = (v1 + v2) + (v1 - v2);

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = (v1(i) + v2(i)) + (v1(i) - v2(i));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::vectorExprAddVectorScalarExprTest() {
    Vector v1 = createVector(10);
    std::iota(std::begin(v1), std::end(v1), 1);

    Vector v2 = createVector(10);
    std::iota(std::begin(v2), std::end(v2), 20);

    Vector result(v1.size());

    double scalar = 0.93847;
    result = (v1 + v2) + scalar * v1;

    // compare to this result
    for (int i = 0; i < 10; ++i) {
        double val = (v1(i) + v2(i)) + v1(i) * scalar;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", val, result(i), 1E-10);
    }
}

void
LinAlgOperatorTest::complexVectorAddExprTest() {
    Vector v1 = createVector(10);
    Vector v2 = createVector(10);
    Vector v3 = createVector(10);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 67);
    std::iota(std::begin(v3), std::end(v3), 978);

    result = (v1 + v2) + v3;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1046, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1061, result(5), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1073, result(9), 1E-10);


    result = (v3 + v2) + (0.1 * v3 + v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1143.8, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1159.3, result(5), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1171.7, result(9), 1E-10);
}

void
LinAlgOperatorTest::scalarMatrixMulTest() {
    Matrix2D m = createMatrix2D();
    Matrix2D result(m.rows(), m.cols());

    result = 0.1 * m;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.1, result(0, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.4, result(1, 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.5, result(2, 0), 1E-10);
}

void
LinAlgOperatorTest::scalarSparseMatrixMulTest() {
    SparseMatrix2D sm = createSparseMatrix2D();
    Matrix2D result(sm.cols(), sm.cols());

    result = 0.1 * sm;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.1, result(0, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.0, result(0, 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.0, result(0, 2), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.0, result(1, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.3, result(1, 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.2, result(1, 2), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.0, result(2, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.0, result(2, 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.6, result(2, 2), 1E-10);
}

void
LinAlgOperatorTest::matrixVectorMulTest() {
    Matrix2D m = createMatrix2D();
    Vector v = createVector(2);
    Vector result(v.size());
    std::iota(std::begin(v), std::end(v), 1);

    result = m * v;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 5.0, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 11.0, result(1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 17.0, result(2), 1E-10);
}

void
LinAlgOperatorTest::sparseMatrixVectorMulTest() {
    SparseMatrix2D sm = createSparseMatrix2D();
    Vector v = createVector(3);
    Vector result(v.size());
    std::iota(std::begin(v), std::end(v), 3);

    result = sm * v;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 3.0, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 22.0, result(1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 30.0, result(2), 1E-10);
}

void
LinAlgOperatorTest::matrixVectorExprMulTest() {
    Matrix2D m = createMatrix2D();
    Vector v1 = createVector(2);
    Vector v2 = createVector(2);
    Vector v3 = createVector(3);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 67);
    std::iota(std::begin(v3), std::end(v3), 978);

    result = m * (v1 + v2) + v3;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1186, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1463, result(1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1740, result(2), 1E-10);
}

void
LinAlgOperatorTest::sparseMatrixVectorExprMulTest() {
    SparseMatrix2D sm = createSparseMatrix2D();
    Vector v1 = createVector(3);
    Vector v2 = createVector(3);
    Vector v3 = createVector(3);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 67);
    std::iota(std::begin(v3), std::end(v3), 978);

    result = sm * (v1 + v2) + v3;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1046, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1333, result(1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1412, result(2), 1E-10);


    result = (v3 + v2) + sm * (0.1 * v3 + v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1143.8, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1548.7, result(1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-vector expression", 1655, result(2), 1E-10);
}

void
LinAlgOperatorTest::sparseSquareMatrixMulSparseSquareMatrixTest() {
    SparseMatrix2D a = createSparseMatrix2D();
    SparseMatrix2D b = createSparseMatrix2D();
    SparseMatrix2D const c{helper::matrixMul(a, b)};

//    c.print();

    // 1st row
    auto expected = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(0, 0), 1E-10);
    expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(0, 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(0, 2), 1E-10);

    // 2ndst row
    expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(1, 0), 1E-10);
    expected = 9.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(1, 1), 1E-10);
    expected = 18.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(1, 2), 1E-10);

    // 3rd row
    expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(2, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(2, 1), 1E-10);
    expected = 36.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(2, 2), 1E-10);
}

void
LinAlgOperatorTest::sparseSquareMatrixMulSparseNonSquareMatrixTest() {
    SparseMatrix2D a(3, 3);
    a(0, 0) = 1.0;
    a(0, 2) = 3.14;
    a(1, 0) = 2.3;
    a(2, 0) = 1.9;
    a(2, 1) = 2.3;
    a.finalize();
//    a.print();

    SparseMatrix2D b(3, 2);
    b(0, 0) = 0.7;
    b(0, 1) = 12.3;
    b(1, 0) = 3.4;
    b(2, 1) = 0.12;
    b.finalize();
//    b.print();

    SparseMatrix2D const c{ helper::matrixMul(a, b) };
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", 3ull, c.rows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", 2ull, c.cols());

//    c.print();


    // 1st row
    auto expected = 0.7;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(0, 0), 1E-10);
    expected = 12.6768;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(0, 1), 1E-10);

    // 2ndst row
    expected = 1.61;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(1, 0), 1E-10);
    expected = 28.29;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(1, 1), 1E-10);

    // 3rd row
    expected = 9.15;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(2, 0), 1E-10);
    expected = 23.37;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(2, 1), 1E-10);
}

void
LinAlgOperatorTest::test3by3MatrixTimes3by3MatrixEqualsIdentity() {
    // example from http://www.purplemath.com/modules/mtrxinvr.htm
    SparseMatrix2D a(3, 3);
    a(0, 0) = 7.0;
    a(0, 1) = -3.0;
    a(0, 2) = -3.0;
    a(1, 0) = -1.0;
    a(1, 1) = 1.0;
    a(2, 0) = -1.0;
    a(2, 2) = 1.0;
    a.finalize();
//    a.print();

    SparseMatrix2D b(3, 3);
    b(0, 0) = 1.0;
    b(0, 1) = 3.0;
    b(0, 2) = 3.0;
    b(1, 0) = 1.0;
    b(1, 1) = 4.0;
    b(1, 2) = 3.0;
    b(2, 0) = 1.0;
    b(2, 1) = 3.0;
    b(2, 2) = 4.0;
    b.finalize();
 //   b.print();

    SparseMatrix2D const c{ helper::matrixMul(a, b) };
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", 3ull, c.rows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", 3ull, c.cols());

//    c.print();


    // 1st row
    auto expected = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(0, 0), 1E-10);
    expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(0, 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(0, 2), 1E-10);

    // 2ndst row
    expected = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(1, 1), 1E-10);
    expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(1, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(1, 2), 1E-10);

    // 3rd row
    expected = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(2, 2), 1E-10);
    expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(2, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix-matrix multiplication", expected, c(2, 1), 1E-10);
}

void
LinAlgOperatorTest::testMatrixTimesMatrixTimesVector() {
    // A * (A * v) == A^{2} * v
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;
    stencil <<
         2, -1,  9,  2,  1,
        -1,  4, -1, -6, -3,
         7, -1,  3, -7, -8,
         3,  5, -8, -9, -3,
         0,  1, -2,  7,  2;
    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);

    Vector v{m.cols()};
    std::iota(std::begin(v), std::end(v), 1);

    // compute m * (m * (m * v))
    Vector result1;
    result1 = m * v;
    result1 = m * result1;
    result1 = m * result1;

    // compute (m * m * m) * v
    SparseMatrix2D m2 = helper::matrixMul(helper::matrixMul(m, m), m);
//    m2.print();
    Vector result2;
    result2 = m2 * v;
    
    CPPUNIT_ASSERT_MESSAGE("matrix-matrix multiplication mismatch", SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-15));
}