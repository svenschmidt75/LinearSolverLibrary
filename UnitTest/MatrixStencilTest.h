/*
* Name  : MatrixStencilTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 10/05/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class MatrixStencilTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MatrixStencilTest);
    CPPUNIT_TEST(TestInitialization);
    CPPUNIT_TEST(TestIndexTo2DMapping);
    CPPUNIT_TEST(Test2DToIndexMapping);
    CPPUNIT_TEST(TestThatThrowsOnEvenStencilDimension);
    CPPUNIT_TEST(TestThatThrowsOnInconsistentStencilDimension);
    CPPUNIT_TEST(TestGeneratedMatrix);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestInitialization();
    void TestIndexTo2DMapping();
    void Test2DToIndexMapping();
    void TestThatThrowsOnEvenStencilDimension();
    void TestThatThrowsOnInconsistentStencilDimension();
    void TestGeneratedMatrix();
};
