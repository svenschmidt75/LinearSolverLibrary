#include "pch.h"

#include "SparseMatrixColumnIteratorTest.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixIterators.h"
#include "LinAlg/ConstRowColumnIterator.h"
#include "LinAlg/ConstColumnIterator.h"


using namespace LinAlg_NS;

namespace {

    SparseMatrix2D CreateSparseMatrix()
    {
        SparseMatrix2D m(3);
        m(0, 0) = 1;
        m(0, 1) = 0;
        m(0, 2) = 3;
        m(1, 0) = 0;
        m(1, 1) = 0;
        m(1, 2) = 6;
        m(2, 0) = 7;
        m(2, 1) = 1;
        m(2, 2) = 0;
        m.finalize();
        return m;
    }

}

void
SparseMatrixColumnIteratorTest::setUp() {}

void
SparseMatrixColumnIteratorTest::tearDown() {}

void
SparseMatrixColumnIteratorTest::RowIteratorSizeEqualsNumberOfRows() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", matrix.rows(), it.maxRows());
}

void
SparseMatrixColumnIteratorTest::RowIteratorReturnsCorrectRow() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current row mismatch", 0ull, it.row());

    it = MatrixIterators::getConstRowColumnIterator(matrix, 2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current row mismatch", 2ull, it.row());
}

void
SparseMatrixColumnIteratorTest::RowIteratorPreIncrementReturnsNextRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, (++it).row());
}

void
SparseMatrixColumnIteratorTest::RowIteratorPostIncrementReturnsNextRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    auto next_it = it++;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, next_it.row());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, it.row());
}

void
SparseMatrixColumnIteratorTest::RowIteratorCannotAdvancePastLastRow() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !it.isValid());
}

void
SparseMatrixColumnIteratorTest::RowIteratorDereferenceReturnsColumnIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
}

void
SparseMatrixColumnIteratorTest::RowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of non-zero elements mismatch", 2ull, it.numberOfNonZeroMatrixElements());
}

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
SparseMatrixColumnIteratorTest::ColumnIteratorDereferenceReturnsCorrectMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *rowit));
}

void
SparseMatrixColumnIteratorTest::ColumnIteratorPreIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    ++rowit;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, rowit.column());
}

void
SparseMatrixColumnIteratorTest::ColumnIteratorPostIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, rowit.column());
    auto val = *rowit++;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, val));
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, rowit.column());
}

void
SparseMatrixColumnIteratorTest::ColumnIteratorCannotAdvancePastLastMatrixElement() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !rowit.isValid());
}

void
SparseMatrixColumnIteratorTest::ColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull() {
    SparseMatrix2D matrix(3);
    matrix(0, 2) = 3;
    matrix(1, 0) = 0;
    matrix(1, 1) = 0;
    matrix(1, 2) = 6;
    matrix(2, 0) = 7;
    matrix(2, 1) = 0;
    matrix(2, 2) = 0;
    matrix.finalize();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, rowit.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
}

void
SparseMatrixColumnIteratorTest::IterateThroughAllElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> column_it = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 1
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    column_it = *++it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 2
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(6.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    it++;
    column_it = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 3
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(7.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !it.isValid());
}