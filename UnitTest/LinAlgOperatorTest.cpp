#include "pch.h"

#include "LinAlgOperatorTest.h"

#include "LinAlg/Vector.h"
#include "LinAlg/BasicEntityOperators.h"
#include "LinAlg/operators.h"
#include "LinAlg/Matrix2D.h"
#include "LinAlg/SparseMatrix2D.h"


using namespace LinAlg_NS;


void
LinAlgOperatorTest::setUp() {}

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
LinAlgOperatorTest::addVectorTest() {
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
LinAlgOperatorTest::subVectorTest() {
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
LinAlgOperatorTest::scalarVectorMulTest() {
    Vector v1 = createVector(10);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);

    result = 0.1 * v1;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 0.1, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1.0, result(9), 1E-10);
}

void
LinAlgOperatorTest::scalarVectorExprMulTest() {
    Vector v1 = createVector(10);
    Vector v2 = createVector(10);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 10);

    result = 0.1 * (v1 + v2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1.1, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 2.9, result(9), 1E-10);
}

void
LinAlgOperatorTest::vectorExprAddTest() {
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
LinAlgOperatorTest::vectorAddVectorMulTest() {
    Vector v1 = createVector(10);
    Vector v2 = createVector(10);
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 67);

    v2 += (v1 * 10.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 77, v2(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 132, v2(5), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 176, v2(9), 1E-10);
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
