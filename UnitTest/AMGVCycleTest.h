/*
* Name  : AMGVCycleTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGVCycleTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGVCycleTest);
    CPPUNIT_TEST(TestThatThrowsOnZeroGridHierarchyDepth);
    CPPUNIT_TEST(Test3LevelGridHierarchyVCycle);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestThatThrowsOnZeroGridHierarchyDepth();
    void Test3LevelGridHierarchyVCycle();
};
