/*
* Name  : AMGWCycleTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 01/19/2014
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGWCycleTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGWCycleTest);
    CPPUNIT_TEST(TestThatThrowsOnZeroGridHierarchyDepth);
    CPPUNIT_TEST(Test1LevelGridHierarchy);
    CPPUNIT_TEST(Test2LevelGridHierarchy);
    CPPUNIT_TEST(Test3LevelGridHierarchy);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestThatThrowsOnZeroGridHierarchyDepth();
    void Test1LevelGridHierarchy();
    void Test2LevelGridHierarchy();
    void Test3LevelGridHierarchy();
};
