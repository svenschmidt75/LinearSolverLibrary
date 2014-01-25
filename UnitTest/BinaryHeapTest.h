/*
* Name  : BinaryHeapTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 01/24/2014
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class BinaryHeapTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(BinaryHeapTest);
    CPPUNIT_TEST(TestBuildHeap1);
    CPPUNIT_TEST(TestBuildHeap2);
    CPPUNIT_TEST(TestBuildHeap3);
    CPPUNIT_TEST(TestBuildHeap4);
    CPPUNIT_TEST(TestBuildHeapComplex);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestBuildHeap1();
    void TestBuildHeap2();
    void TestBuildHeap3();
    void TestBuildHeap4();
    void TestBuildHeapComplex();
};
