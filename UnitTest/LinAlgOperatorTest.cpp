#include "LinAlgOperatorTest.h"

#include "LinAlg/Vector.h"
#include "LinAlg/operators.h"
#include "LinAlg/Matrix2D.h"
#include "LinAlg/SparseMatrix2D.h"

#include <numeric>


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
        m(2, 2) = 6;
        m.finalize();
        return m;
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.0, result(1, 2), 1E-10);
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

    //    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.1, result(0, 0), 1E-10);
}

void
LinAlgOperatorTest::sparseMatrixVectorMulTest() {
    SparseMatrix2D sm = createSparseMatrix2D();
    Vector v = createVector(3);
    Vector result(v.size());

    std::iota(std::begin(v), std::end(v), 1);

    result = sm * v;

//    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in matrix expression", 0.1, result(0, 0), 1E-10);
}
