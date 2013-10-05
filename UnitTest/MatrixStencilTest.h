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
    CPPUNIT_TEST(Test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test1();
};
