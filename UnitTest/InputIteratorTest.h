/*
 * Name  : InputIteratorTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 10/26/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class InputIteratorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(InputIteratorTest);
    CPPUNIT_TEST(TestForwardIterator);
    CPPUNIT_TEST(TestPostIncrement);
    CPPUNIT_TEST(TestPostIncrementWithAssignment);
    CPPUNIT_TEST(TestPreIncrement);
    CPPUNIT_TEST(TestPreIncrementWithAssignment);
    CPPUNIT_TEST(TestIteratorEquality);
    CPPUNIT_TEST(TestIteratorInEquality);
    CPPUNIT_TEST(TestIteratorCopyConstruction);
    CPPUNIT_TEST(TestIteratorAssignment);
    CPPUNIT_TEST(TestIteratorEnd);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestForwardIterator();
    void TestPostIncrement();
    void TestPostIncrementWithAssignment();
    void TestPreIncrement();
    void TestPreIncrementWithAssignment();
    void TestIteratorEquality();
    void TestIteratorInEquality();
    void TestIteratorCopyConstruction();
    void TestIteratorAssignment();
    void TestIteratorEnd();
};

