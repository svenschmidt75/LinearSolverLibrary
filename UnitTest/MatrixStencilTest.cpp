#include "pch.h"

#include "MatrixStencilTest.h"

#include "LinAlg/MatrixStencil.h"


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
MatrixStencilTest::TestGeneratedMatrix() {
    MatrixStencil stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    SparseMatrix2D const & m = stencil.generateMatrix(5);
    double expected = 8;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, 0));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil matrix error", expected, m(0, -1));
}
