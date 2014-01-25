/*
* Name  : PriorityArrayTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 01/25/2014
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class PriorityArrayTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(PriorityArrayTest);
    CPPUNIT_TEST(Test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test1();
};
