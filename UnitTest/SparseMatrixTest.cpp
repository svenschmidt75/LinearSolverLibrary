#include "pch.h"

#include "SparseMatrixTest.h"


using namespace LinAlg_NS;


void
SparseMatrixTest::SparseMatrixRowIteratorTest() {}

void
SparseMatrixTest::tearDown() {}

namespace {

    SparseMatrix2D createSquareSparseMatrix() {
        SparseMatrix2D matrix{3};

        // 0 0 3
        // 0 8 6
        // 7 0 0

        matrix(0, 2) = 3;
        matrix(1, 0) = 0;
        matrix(1, 1) = 8;
        matrix(1, 2) = 6;
        matrix(2, 0) = 7;
        matrix(2, 1) = 0;
        matrix(2, 2) = 0;
        matrix.finalize();
        return matrix;
    }

    SparseMatrix2D createNonSquareSparseMatrix() {
        SparseMatrix2D m(3, 2);

        // 1 2
        // 0 4
        // 5 0

        m(0, 0) = 1;
        m(0, 1) = 2;
        m(1, 1) = 4;
        m(2, 0) = 5;
        m.finalize();
        return m;
    }

}

void
SparseMatrixTest::testCreateSquareSparseMatrix() {
    SparseMatrix2D m = createSquareSparseMatrix();
    auto expected = 3ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, m.rows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", expected, m.cols());
}

void
SparseMatrixTest::testCreateNonSquareSparseMatrix() {
    SparseMatrix2D m = createNonSquareSparseMatrix();
    auto expected = 3ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, m.rows());
    expected = 2ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", expected, m.cols());
}

void
SparseMatrixTest::testAccessToNonExistingElementReturnsNull() {
    SparseMatrix2D const m1 = createSquareSparseMatrix();
    auto expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, m1(0, 1), 1E-10);

    SparseMatrix2D const m2 = createSquareSparseMatrix();
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, m2(1, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, m2(2, 1), 1E-10);
}

void
SparseMatrixTest::sparseSquareMatrixTranspose() {
    auto const m = createSquareSparseMatrix();
    auto const mTransposed = helper::transpose(m);
    auto expected = m.rows();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, mTransposed.cols());
    expected = m.cols();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", expected, mTransposed.rows());

    {
        auto expected = m(0, 2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, mTransposed(2, 0), 1E-10);

        expected = m(1, 1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, mTransposed(1, 1), 1E-10);

        expected = m(1, 2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, mTransposed(2, 1), 1E-10);

        expected = m(2, 0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, mTransposed(0, 2), 1E-10);
    }
}

void
SparseMatrixTest::sparseNonSquareMatrixTranspose() {
    auto const m = createNonSquareSparseMatrix();
    auto const mTransposed = helper::transpose(m);
    auto expected = m.rows();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, mTransposed.cols());
    expected = m.cols();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", expected, mTransposed.rows());

    {
        auto expected = m(0, 0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, mTransposed(0, 0), 1E-10);

        expected = m(0, 1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, mTransposed(1, 0), 1E-10);

        expected = m(1, 1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, mTransposed(1, 1), 1E-10);

        expected = m(2, 0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, mTransposed(0, 2), 1E-10);
    }
}