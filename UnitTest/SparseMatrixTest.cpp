#include "pch.h"

#include "SparseMatrixTest.h"


using namespace LinAlg_NS;


void
SparseMatrixTest::SparseMatrixRowIteratorTest() {}

void
SparseMatrixTest::tearDown() {}

namespace {

    SparseMatrix2D create3by3SparseMatrix() {
        SparseMatrix2D matrix{3};

        // 0        0        3
        // 0        8        6
        // 7        0        0

        matrix(0, 2) = 3;
        matrix(1, 1) = 8;
        matrix(1, 2) = 6;
        matrix(2, 0) = 7;
        matrix.finalize();
//        matrix.print();
        return matrix;
    }

    SparseMatrix2D create3by2SparseMatrix() {
        SparseMatrix2D m{3, 2};

        // 1        2
        // 0        4
        // 5        0

        m(0, 0) = 1;
        m(0, 1) = 2;
        m(1, 1) = 4;
        m(2, 0) = 5;
        m.finalize();
//        m.print();
        return m;
    }

    SparseMatrix2D create5by5SparseMatrix() {
        SparseMatrix2D m{5, 5};

        // 1        0        0        2        0
        // 3        4        0        5        0
        // 6        0        7        8        9
        // 0        0       10       11        0
        // 0        0        0        0       12

        m(0, 0) = 1.0;
        m(0, 3) = 2.0;
        m(1, 0) = 3.0;
        m(1, 1) = 4.0;
        m(1, 3) = 5.0;
        m(2, 0) = 6.0;
        m(2, 2) = 7.0;
        m(2, 3) = 8.0;
        m(2, 4) = 9.0;
        m(3, 2) = 10.0;
        m(3, 3) = 11.0;
        m(4, 4) = 12.0;
        m.finalize();
//        m.print();
        return m;
    }

    SparseMatrix2D create3by5SparseMatrix() {
        SparseMatrix2D m{3, 5};

        // 1        0        0        2        0
        // 3        4        0        5        0
        // 6        0        7        8        9

        m(0, 0) = 1.0;
        m(0, 3) = 2.0;
        m(1, 0) = 3.0;
        m(1, 1) = 4.0;
        m(1, 3) = 5.0;
        m(2, 0) = 6.0;
        m(2, 2) = 7.0;
        m(2, 3) = 8.0;
        m(2, 4) = 9.0;
        m.finalize();
//        m.print();
        return m;
    }

}

void
SparseMatrixTest::testCreateSquareSparseMatrix() {
    SparseMatrix2D m = create3by3SparseMatrix();
    auto expected = 3ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, m.rows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", expected, m.cols());
}

void
SparseMatrixTest::testCreateNonSquareSparseMatrix() {
    SparseMatrix2D m = create3by2SparseMatrix();
    auto expected = 3ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, m.rows());
    expected = 2ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", expected, m.cols());
}

void
SparseMatrixTest::testAccessToNonExistingElementReturnsNull() {
    SparseMatrix2D const m1 = create3by3SparseMatrix();
    auto expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, m1(0, 1), 1E-10);

    SparseMatrix2D const m2 = create3by3SparseMatrix();
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, m2(1, 0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", expected, m2(2, 1), 1E-10);
}

void
SparseMatrixTest::sparseSquareMatrixTranspose() {
    auto const m = create3by3SparseMatrix();
    auto const mTransposed = helper::transpose(m);
    auto expected = m.rows();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, mTransposed.cols());
    expected = m.cols();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", expected, mTransposed.rows());

    {
        auto exp = m(0, 2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", exp, mTransposed(2, 0), 1E-10);

        exp = m(1, 1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", exp, mTransposed(1, 1), 1E-10);

        exp = m(1, 2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", exp, mTransposed(2, 1), 1E-10);

        exp = m(2, 0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", exp, mTransposed(0, 2), 1E-10);
    }
}

void
SparseMatrixTest::sparseNonSquareMatrixTranspose() {
    auto const m = create3by2SparseMatrix();
    auto const mTransposed = helper::transpose(m);
    auto expected = m.rows();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, mTransposed.cols());
    expected = m.cols();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of columns mismatch", expected, mTransposed.rows());

    {
        auto exp = m(0, 0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", exp, mTransposed(0, 0), 1E-10);

        exp = m(0, 1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", exp, mTransposed(1, 0), 1E-10);

        exp = m(1, 1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", exp, mTransposed(1, 1), 1E-10);

        exp = m(2, 0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix element mismatch", exp, mTransposed(0, 2), 1E-10);
    }
}

void
SparseMatrixTest::testColumnIndicesFor5by5SparseMatrix() {
    auto const m = create5by5SparseMatrix();

    using size_type = SparseMatrix2D::size_type;

    // row 0
    auto column_indices = m.getNonZeroColumnIndicesForRow(0);
    size_type expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 3;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 3", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 2ull, column_indices.size());

    // row 1
    column_indices = m.getNonZeroColumnIndicesForRow(1);
    expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 1", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 3;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 3", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 3ull, column_indices.size());

    // row 4
    column_indices = m.getNonZeroColumnIndicesForRow(4);
    expected = 4;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 4", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 1ull, column_indices.size());
}

void
SparseMatrixTest::testRowIndicesFor5by5SparseMatrix() {
    auto const m = create5by5SparseMatrix();

    using size_type = SparseMatrix2D::size_type;

    // column 0
    auto row_indices = m.getNonZeroRowIndicesForColumn(0);
    size_type expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 0", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 3ull, row_indices.size());

    // column 1
    row_indices = m.getNonZeroRowIndicesForColumn(1);
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 1ull, row_indices.size());

    // column 4
    row_indices = m.getNonZeroRowIndicesForColumn(4);
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 4;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 4", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 2ull, row_indices.size());
}

void
SparseMatrixTest::testColumnIndicesFor3by5SparseMatrix() {
    auto const m = create3by5SparseMatrix();

    using size_type = SparseMatrix2D::size_type;

    // row 0
    auto column_indices = m.getNonZeroColumnIndicesForRow(0);
    size_type expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 3;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 3", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 2ull, column_indices.size());

    // row 1
    column_indices = m.getNonZeroColumnIndicesForRow(1);
    expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 1", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 3;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 3", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 3ull, column_indices.size());

    // row 2
    column_indices = m.getNonZeroColumnIndicesForRow(2);
    expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 2", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 3;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 3", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 4;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 4", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 4ull, column_indices.size());
}

void
SparseMatrixTest::testRowIndicesFor3by5SparseMatrix() {
    auto const m = create3by5SparseMatrix();

    using size_type = SparseMatrix2D::size_type;

    // column 0
    auto row_indices = m.getNonZeroRowIndicesForColumn(0);
    size_type expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 0", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 3ull, row_indices.size());

    // column 1
    row_indices = m.getNonZeroRowIndicesForColumn(1);
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 1ull, row_indices.size());

    // column 4
    row_indices = m.getNonZeroRowIndicesForColumn(4);
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 1ull, row_indices.size());
}

void
SparseMatrixTest::testColumnIndicesForSquareMatrixMatrixMul() {
    SparseMatrix2D m(3);

    // 1 0 0
    // 0 3 2
    // 0 0 6

    m(0, 0) = 1;
    m(1, 1) = 3;
    m(1, 2) = 2;
    m(2, 2) = 6;
    m.finalize();

    auto const & c = m * m;
    
    // 1        0        0
    // 0        9       18
    // 0        0       36
//    c.print();

    using size_type = SparseMatrix2D::size_type;

    // row 0
    auto column_indices = c.getNonZeroColumnIndicesForRow(0);
    size_type expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 1ull, column_indices.size());

    // row 1
    column_indices = c.getNonZeroColumnIndicesForRow(1);
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 1", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 2", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 2ull, column_indices.size());

    // row 2
    column_indices = c.getNonZeroColumnIndicesForRow(2);
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 2", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 1ull, column_indices.size());
}

void
SparseMatrixTest::testRowIndicesForSquareMatrixMatrixMul() {
    SparseMatrix2D m(3);

    // 1 0 0
    // 0 3 2
    // 0 0 6

    m(0, 0) = 1;
    m(1, 1) = 3;
    m(1, 2) = 2;
    m(2, 2) = 6;
    m.finalize();

    auto const & c = m * m;

    // 1        0        0
    // 0        9       18
    // 0        0       36
//    c.print();

    using size_type = SparseMatrix2D::size_type;

    // column 0
    auto row_indices = c.getNonZeroRowIndicesForColumn(0);
    size_type expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 0", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 1ull, row_indices.size());

    // column 1
    row_indices = c.getNonZeroRowIndicesForColumn(1);
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 1ull, row_indices.size());

    // column 2
    row_indices = c.getNonZeroRowIndicesForColumn(2);
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 2ull, row_indices.size());
}

void
SparseMatrixTest::testColumnIndicesForNonSquareMatrixMatrixMul() {
    SparseMatrix2D const lhs = create3by5SparseMatrix();
    SparseMatrix2D const rhs = create5by5SparseMatrix();

    auto const & c = lhs * rhs;

    //  1.000   0.000  20.000  24.000   0.000
    // 15.000  16.000  50.000  81.000   0.000
    // 48.000   0.000 129.000 156.000 171.000

    using size_type = SparseMatrix2D::size_type;

    // row 0
    auto column_indices = c.getNonZeroColumnIndicesForRow(0);
    size_type expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 3;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 3ull, column_indices.size());

    // row 1
    column_indices = c.getNonZeroColumnIndicesForRow(1);
    expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 1", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 2", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 3;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 3", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 4ull, column_indices.size());

    // row 2
    column_indices = c.getNonZeroColumnIndicesForRow(2);
    expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 0", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 2", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 3;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 3", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    expected = 4;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find column 4", std::find(std::cbegin(column_indices), std::cend(column_indices), expected) != std::cend(column_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of column indices mismatch", 4ull, column_indices.size());
}

void
SparseMatrixTest::testRowIndicesForNonSquareMatrixMatrixMul() {
    SparseMatrix2D const lhs = create3by5SparseMatrix();
    SparseMatrix2D const rhs = create5by5SparseMatrix();

    auto const & c = lhs * rhs;

    //  1.000   0.000  20.000  24.000   0.000
    // 15.000  16.000  50.000  81.000   0.000
    // 48.000   0.000 129.000 156.000 171.000
    // 1        0        0
    // 0        9       18
    // 0        0       36
 
    using size_type = SparseMatrix2D::size_type;

    // column 0
    auto row_indices = c.getNonZeroRowIndicesForColumn(0);
    size_type expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 0", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 3ull, row_indices.size());

    // column 1
    row_indices = c.getNonZeroRowIndicesForColumn(1);
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 1ull, row_indices.size());

    // column 2
    row_indices = c.getNonZeroRowIndicesForColumn(2);
    expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 0", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 3ull, row_indices.size());

    // column 3
    row_indices = c.getNonZeroRowIndicesForColumn(3);
    expected = 0;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 0", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 1;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 1", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 3ull, row_indices.size());

    // column 4
    row_indices = c.getNonZeroRowIndicesForColumn(4);
    expected = 2;
    CPPUNIT_ASSERT_MESSAGE("Couldn't find row 2", std::find(std::cbegin(row_indices), std::cend(row_indices), expected) != std::cend(row_indices));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of row indices mismatch", 1ull, row_indices.size());
}
