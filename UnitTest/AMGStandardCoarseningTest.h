/*
* Name  : AMGStandardCoarseningTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 10/12/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGStandardCoarseningTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGStandardCoarseningTest);
    CPPUNIT_TEST(Test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test1();
};
