/*
* Name  : AMGVariableCardinalityTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGVariableCardinalityTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGVariableCardinalityTest);
    CPPUNIT_TEST(Test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test1();
};
