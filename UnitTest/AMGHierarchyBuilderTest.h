/*
* Name  : AMGHierarchyBuilderTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 02/02/2014
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGHierarchyBuilderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGHierarchyBuilderTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test1();
};
