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
    CPPUNIT_TEST(Test1LevelGridHierarchyVCycle);
    CPPUNIT_TEST(Test3LevelGridHierarchyVCycle);
    CPPUNIT_TEST(Test2LevelGridHierarchyGamma2);
    CPPUNIT_TEST(Test2LevelGridHierarchyGamma3);
    CPPUNIT_TEST(Test3LevelGridHierarchyGamma2);
    CPPUNIT_TEST(Test3LevelGridHierarchyGamma3);
    CPPUNIT_TEST(Test4LevelGridHierarchyGamma2);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestThatThrowsOnZeroGridHierarchyDepth();
    void Test1LevelGridHierarchyVCycle();
    void Test3LevelGridHierarchyVCycle();
    void Test2LevelGridHierarchyGamma2();
    void Test2LevelGridHierarchyGamma3();
    void Test3LevelGridHierarchyGamma2();
    void Test3LevelGridHierarchyGamma3();
    void Test4LevelGridHierarchyGamma2();
};
