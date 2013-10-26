/*
* Name  : VariableInfluenceAccessorTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 10/26/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class VariableInfluenceAccessorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(VariableInfluenceAccessorTest);
    CPPUNIT_TEST(Test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test1();
};
