/*
* Name  : AMGPolicyTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGPolicyTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGPolicyTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test1();
};
