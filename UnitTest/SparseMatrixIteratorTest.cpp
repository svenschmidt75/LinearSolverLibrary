#include "pch.h"

#include "SparseMatrixIteratorTest.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/iterators.h"


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
        m(2, 1) = 0;
        m(2, 2) = 0;
        m.finalize();
        return m;
    }

}

void
SparseMatrixIteratorTest::setUp() {}

void
SparseMatrixIteratorTest::tearDown() {}

void
SparseMatrixIteratorTest::RowIteratorSizeEqualsNumberOfRows() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column mismatch", matrix.rows(), it.maxRows());
}

void
SparseMatrixIteratorTest::RowIteratorReturnsCorrectRow() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current row mismatch", 0ull, it.row());
}

void
SparseMatrixIteratorTest::RowIteratorPreIncrementReturnsNextRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, (++it).row());
}

void
SparseMatrixIteratorTest::RowIteratorPostIncrementReturnsNextRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    auto next_it = it++;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, next_it.row());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, it.row());
}

void
SparseMatrixIteratorTest::RowIteratorDereferenceReturnsColumnIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
}

void
SparseMatrixIteratorTest::ColumnIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
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
SparseMatrixIteratorTest::ColumnIteratorDereferenceReturnsCorrectMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *rowit));
}

void
SparseMatrixIteratorTest::ColumnIteratorPreIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    ++rowit;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Element mismatch", 2ull, rowit.column());
}

void
SparseMatrixIteratorTest::ColumnIteratorPostIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Element mismatch", 0ull, rowit.column());
    auto val = rowit++;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, val));
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Element mismatch", 2ull, rowit.column());
}

void
SparseMatrixIteratorTest::ColumnIteratorCannotAdvancePastLastMatrixElement() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator invalid", rowit.next());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator valid", !rowit.next());
}
