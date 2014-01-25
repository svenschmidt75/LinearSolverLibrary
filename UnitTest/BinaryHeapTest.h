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
    CPPUNIT_TEST(TestBuildHeap5);
    CPPUNIT_TEST(TestBuildHeapComplex1);
    CPPUNIT_TEST(TestBuildHeapComplex2);
    CPPUNIT_TEST(TestInsertSimple);
    CPPUNIT_TEST(TestInsertComplex);
    CPPUNIT_TEST(TestRemoveThrowsWhenEmpty);
    CPPUNIT_TEST(TestRemoveFromEnd);
    CPPUNIT_TEST(TestRemoveInside1);
    CPPUNIT_TEST(TestRemoveInside2);
    CPPUNIT_TEST(TestRemoveRoot);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestBuildHeap1();
    void TestBuildHeap2();
    void TestBuildHeap3();
    void TestBuildHeap4();
    void TestBuildHeap5();
    void TestBuildHeapComplex1();
    void TestBuildHeapComplex2();
    void TestInsertSimple();
    void TestInsertComplex();
    void TestRemoveThrowsWhenEmpty();
    void TestRemoveFromEnd();
    void TestRemoveInside1();
    void TestRemoveInside2();
    void TestRemoveRoot();
};
