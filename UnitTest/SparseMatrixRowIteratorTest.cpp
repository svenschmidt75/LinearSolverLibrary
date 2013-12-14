#include "pch.h"

#include "SparseMatrixRowIteratorTest.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/ConstColumnRowIterator.h"
#include "LinAlg/ConstRowIterator.h"
#include "LinAlg/MatrixIterators.h"


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
SparseMatrixRowIteratorTest::setUp() {}

void
SparseMatrixRowIteratorTest::tearDown() {}

void
SparseMatrixRowIteratorTest::ColumnIteratorSizeEqualsNumberOfColumns() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", matrix.cols(), it.maxColumns());
}

void
SparseMatrixRowIteratorTest::ColumnIteratorReturnsCorrectColumn() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current column mismatch", 0ull, it.column());

    it = MatrixIterators::getConstColumnRowIterator(matrix, 2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current column mismatch", 2ull, it.column());
}

void
SparseMatrixRowIteratorTest::ColumnIteratorPreIncrementReturnsNextColumnIterator() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, (++it).column());
}

void
SparseMatrixRowIteratorTest::ColumnIteratorPostIncrementReturnsNextColumnIterator() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    auto next_it = it++;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, next_it.column());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, it.column());
}

void
SparseMatrixRowIteratorTest::ColumnIteratorCannotAdvancePastLastColumn() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", bool(it) == true);
    it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !it.isValid());
}

void
SparseMatrixRowIteratorTest::ColumnIteratorDereferenceReturnsRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    ConstRowIterator<SparseMatrix2D> rowit = *it;
}

void
SparseMatrixRowIteratorTest::ColumnIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentColumn() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of non-zero elements mismatch", 2ull, it.numberOfNonZeroMatrixElements());
    it++;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of non-zero elements mismatch", 1ull, it.numberOfNonZeroMatrixElements());
    it++;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of non-zero elements mismatch", 2ull, it.numberOfNonZeroMatrixElements());
}

void
SparseMatrixRowIteratorTest::RowIteratorInitializesToFirstElement() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    auto rowIterator = *it;
    auto expected = 0ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong initial row", expected, rowIterator.row());
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", bool(rowIterator) == true);

    // move to next column
    it++;
    rowIterator = *it;
    expected = 2ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong initial row", expected, rowIterator.row());
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", bool(rowIterator) == true);

    // move to next column
    it++;
    rowIterator = *it;
    expected = 0ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong initial row", expected, rowIterator.row());
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", bool(rowIterator) == true);
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
SparseMatrixRowIteratorTest::RowIteratorDereferenceReturnsCorrectMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    ConstRowIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *rowit));
}

void
SparseMatrixRowIteratorTest::RowIteratorPreIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    ConstRowIterator<SparseMatrix2D> rowit = *it;
    ++rowit;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(7.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 2ull, rowit.row());
}

void
SparseMatrixRowIteratorTest::RowIteratorPostIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    ConstRowIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, rowit.row());
    auto val = *rowit++;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, val));
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(7.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 2ull, rowit.row());
}

void
SparseMatrixRowIteratorTest::RowIteratorCannotAdvancePastLastMatrixElement() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    ConstRowIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !rowit.isValid());

    rowit = *++it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !rowit.isValid());
}

void
SparseMatrixRowIteratorTest::RowIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull() {
    SparseMatrix2D matrix(3);
    matrix(0, 2) = 3;
    matrix(1, 0) = 0;
    matrix(1, 1) = 0;
    matrix(1, 2) = 6;
    matrix(2, 0) = 7;
    matrix(2, 1) = 0;
    matrix(2, 2) = 0;
    matrix.finalize();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix, 0);
    ConstRowIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 2ull, rowit.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(7.0, *rowit));

    it = MatrixIterators::getConstColumnRowIterator(matrix, 1);
    rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !rowit.isValid());

    it = MatrixIterators::getConstColumnRowIterator(matrix, 2);
    rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, rowit.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    ++rowit;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, rowit.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(6.0, *rowit));
}

void
SparseMatrixRowIteratorTest::IterateThroughAllElements() {
    auto matrix = CreateSparseMatrix();
    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(matrix);
    ConstRowIterator<SparseMatrix2D> row_it = *it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", it.isValid());

    // column 1
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", row_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, row_it.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *row_it));
    row_it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", row_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 2ull, row_it.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(7.0, *row_it));
    row_it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !row_it.isValid());

    row_it = *++it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", it.isValid());

    // row 2
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", row_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 2ull, row_it.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *row_it));
    row_it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !row_it.isValid());

    it++;
    row_it = *it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", it.isValid());

    // row 3
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", row_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, row_it.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *row_it));
    row_it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", row_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, row_it.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(6.0, *row_it));
    row_it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !row_it.isValid());

    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !it.isValid());
}

void
SparseMatrixRowIteratorTest::TestRowIteratorForMatrixWithOneElement() {
    SparseMatrix2D m(3, 4);
    m(0, 1) = 1.14;
    m(1, 2) = 2.14;
    m(2, 3) = 3.14;
    m.finalize();

    ConstColumnRowIterator<SparseMatrix2D> it = MatrixIterators::getConstColumnRowIterator(m);
    ConstRowIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !rowit.isValid());
    CPPUNIT_ASSERT_THROW_MESSAGE("Row iterator pre-increment operator should have thrown", ++rowit, std::runtime_error);

    rowit = *++it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", bool(rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, rowit.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.14, *rowit));

    rowit = *++it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", bool(rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, rowit.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(2.14, *rowit));

    rowit = *++it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", bool(rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 2ull, rowit.row());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.14, *rowit));
}