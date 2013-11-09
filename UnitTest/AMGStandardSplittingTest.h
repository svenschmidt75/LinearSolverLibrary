/*
* Name  : AMGStandardSplittingTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 11/09/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGStandardSplittingTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGStandardSplittingTest);
    CPPUNIT_TEST(Test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test1();
};
