/*
* Name  : AMGSolverTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 02/02/2014
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGSolverTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGSolverTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test1();
};
