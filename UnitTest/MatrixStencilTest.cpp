#include "pch.h"

#include "MatrixStencilTest.h"

#include "LinAlg/MatrixStencil.h"
#include "LinAlg/iterators.h"


using namespace LinAlg_NS;


void
MatrixStencilTest::setUp() {}

void
MatrixStencilTest::tearDown() {}

void
MatrixStencilTest::TestInitialization() {
    MatrixStencil stencil;
    stencil << -1, 3, -17.5;
    double expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil error", expected, stencil.values_[0]);
    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil error", expected, stencil.values_[1]);
    expected = -17.5;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil error", expected, stencil.values_[2]);
}

void
MatrixStencilTest::TestIndexTo2DMapping() {
    MatrixStencil stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    short expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.mapTo2D(4)));
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.mapTo2D(4)));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.mapTo2D(0)));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.mapTo2D(0)));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.mapTo2D(6)));
    expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.mapTo2D(6)));

    expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.mapTo2D(5)));
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.mapTo2D(5)));
}

void
MatrixStencilTest::Test2DToIndexMapping() {
    MatrixStencil stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    unsigned short expected = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, stencil.mapToIndex(0, 0));

    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, stencil.mapToIndex(-1, -1));

    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, stencil.mapToIndex(-1, 0));

    expected = 7;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, stencil.mapToIndex(0, 1));
}

void
MatrixStencilTest::TestThatThrowsOnEvenStencilDimension() {
    MatrixStencil stencil;
    stencil << -1, -1, -1,
               -1,  8, -1;

    CPPUNIT_ASSERT_THROW_MESSAGE("Matrix stencil must have odd dimension", stencil.generateMatrix(5), std::runtime_error);
}

void
MatrixStencilTest::TestThatThrowsOnInconsistentStencilDimension() {
    MatrixStencil stencil;
    stencil << -1, -1, -1,
               -1, 8, -1,
               -1;

    CPPUNIT_ASSERT_THROW_MESSAGE("Matrix stencil must be of type n x n", stencil.generateMatrix(5), std::runtime_error);
}

void
MatrixStencilTest::TestGenerated3By3MatrixForFivePointStencil() {
    MatrixStencil stencil;
    stencil <<  0, -1,  0,
               -1,  4, -1,
                0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(m);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row count mismatch", m.rows(), it.maxRows());

    // row 1
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 3ull, it++.numberOfNonZeroMatrixElements());

    double expected = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 0));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 1));
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 2));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 3));

    // row 2
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 4ull, it++.numberOfNonZeroMatrixElements());

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 0));
    expected = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 1));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 2));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 4));

    // row 3
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 3ull, it++.numberOfNonZeroMatrixElements());

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(2, 1));
    expected = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(2, 2));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(2, 5));

    // row 4
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 4ull, it++.numberOfNonZeroMatrixElements());

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 0));
    expected = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 3));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 4));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 6));

    // row 5
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 5ull, it++.numberOfNonZeroMatrixElements());

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 1));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 3));
    expected = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 4));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 5));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 7));

    // row 6
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 4ull, it++.numberOfNonZeroMatrixElements());

    // row 7
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 3ull, it++.numberOfNonZeroMatrixElements());

    // row 8
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 4ull, it++.numberOfNonZeroMatrixElements());

    // row 9
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 3ull, it++.numberOfNonZeroMatrixElements());
}

void
MatrixStencilTest::TestGenerated3By3MatrixForNinePointStencil() {
    MatrixStencil stencil;
    stencil << -1, -4, -1,
               -4, 20, -4,
               -1, -4, -1;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    Matrix2D reference_matrix(9, 9);
    reference_matrix <<
        20, -4,  0, -4, -1,  0,  0,  0,  0,
        -4, 20, -4, -1, -4, -1,  0,  0,  0,
         0, -4, 20,  0, -1, -4,  0,  0,  0,
        -4, -1,  0, 20, -4,  0, -4, -1,  0,
        -1, -4, -1, -4, 20, -4, -1, -4, -1,
         0, -1, -4,  0, -4, 20,  0, -1, -4,
         0, -4,  0, -4, -1,  0, 20, -4,  0,
         0, -4,  0, -1, -4, -1, -4, 20, -4,
         0, -4,  0,  0, -1, -4,  0, -4, 20;


    ConstRowIterator<SparseMatrix2D> it = iterators::getConstRowIterator(m);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row count mismatch", m.rows(), it.maxRows());

    // row 1
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 4ull, it++.numberOfNonZeroMatrixElements());

    double expected = 20;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 0));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 1));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 3));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 4));

    // row 2
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 6ull, it++.numberOfNonZeroMatrixElements());

    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 0));
    expected = 20;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 1));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 2));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 3));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 4));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(1, 5));

    // row 3
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 4ull, it++.numberOfNonZeroMatrixElements());

    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(2, 1));
    expected = 20;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(2, 2));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(2, 4));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(2, 5));

    // row 4
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 6ull, it++.numberOfNonZeroMatrixElements());

    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 0));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 1));
    expected = 20;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 3));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 4));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 6));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(3, 7));

    // row 5
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 9ull, it++.numberOfNonZeroMatrixElements());

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 0));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 1));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 2));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 3));
    expected = 20;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 4));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 5));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 6));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 7));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(4, 8));

    // row 6
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 6ull, it++.numberOfNonZeroMatrixElements());

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(5, 1));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(5, 2));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(5, 4));
    expected = 20;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(5, 5));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(5, 7));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(5, 8));

    // row 7
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 4ull, it++.numberOfNonZeroMatrixElements());

    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(6, 3));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(6, 4));
    expected = 20;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(6, 6));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(6, 7));

    // row 8
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 4ull, it++.numberOfNonZeroMatrixElements());

    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(6, 3));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(6, 4));
    expected = 20;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(6, 6));
    expected = -4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(6, 7));

    // row 9
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 3ull, it++.numberOfNonZeroMatrixElements());
}

void
MatrixStencilTest::TestGenerated3By3MatrixFor25PointStencil() {
    MatrixStencil stencil;
    stencil << 0, 0, -1, 0, 0,
        0, 0, -4, 0, 0,
        -1, -4, 20, -4, -1,
        0, 0, -4, 0, 0,
        0, 0, -1, 0, 0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);
    double expected = 8;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 0));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, -1));
}

void
MatrixStencilTest::TestGenerated4By4MatrixForFivePointStencil() {
    MatrixStencil stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(4 * 4);
    double expected = 8;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 0));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, -1));
}
