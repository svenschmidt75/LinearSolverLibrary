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
SparseMatrixIteratorTest::ColumnIteratorSizeEqualsNumberOfColumns() {
    auto matrix = CreateSparseMatrix();
    ConstColumnIterator<SparseMatrix2D> it = iterators::getConstIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column mismatch", 3ull, it.maxColumns());
}

void
SparseMatrixIteratorTest::ColumnIteratorReturnsCorrectColumn() {
    auto matrix = CreateSparseMatrix();
    ConstColumnIterator<SparseMatrix2D> it = iterators::getConstIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current column mismatch", 0ull, it.column());
}

void
SparseMatrixIteratorTest::ColumnIteratorPreIncrementReturnsNextColumnIterator() {
    auto matrix = CreateSparseMatrix();
    ConstColumnIterator<SparseMatrix2D> it = iterators::getConstIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, (++it).column());
}

void
SparseMatrixIteratorTest::ColumnIteratorPostIncrementReturnsNextColumnIterator() {
    auto matrix = CreateSparseMatrix();
    ConstColumnIterator<SparseMatrix2D> it = iterators::getConstIterator(matrix);
    auto next_it = it++;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, next_it.column());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, it.column());
}

void
SparseMatrixIteratorTest::ColumnIteratorDereferenceReturnsRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstColumnIterator<SparseMatrix2D> it = iterators::getConstIterator(matrix);
    //ConstRowIterator<SparseMatrix2D> rowit = *it;
}
