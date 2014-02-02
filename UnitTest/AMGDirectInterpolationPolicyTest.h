/*
* Name  : AMGDirectInterpolationPolicyTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGDirectInterpolationPolicyTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGDirectInterpolationPolicyTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test1();
};
