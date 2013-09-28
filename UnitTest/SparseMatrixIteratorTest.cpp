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
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;
        m(2, 0) = 7;
        m(2, 1) = 8;
        m(2, 2) = 9;
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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column mismatch", 3ull, it.maxRows());
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
SparseMatrixIteratorTest::RowIteratorDereferenceReturnsRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
}
