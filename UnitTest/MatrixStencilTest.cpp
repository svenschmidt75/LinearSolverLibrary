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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.MapTo2D(4)));
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.MapTo2D(4)));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.MapTo2D(0)));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.MapTo2D(0)));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.MapTo2D(6)));
    expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.MapTo2D(6)));

    expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.MapTo2D(5)));
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.MapTo2D(5)));
}

void
MatrixStencilTest::TestThatThrowsOnEvenStencilDimension() {
//    CPPUNIT_ASSERT_THROW_MESSAGE("Invalid access axpected", MatrixStencil stencil(2), std::runtime_error);
}
