/*
 * Name  : LUDecompositionTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 08/11/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class LUDecompositionTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LUDecompositionTest);
    CPPUNIT_TEST(Test3by3_1);
    CPPUNIT_TEST(Test3by3_2);
    CPPUNIT_TEST(Test3by3_3);
    CPPUNIT_TEST(Test3by3_4);
    CPPUNIT_TEST(Test3by3_5);
    CPPUNIT_TEST(Test4by4_1);
    CPPUNIT_TEST(Test4by4_2);
    CPPUNIT_TEST(Test4by4_3);
    CPPUNIT_TEST(Test4by4_4);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test3by3_1();
    void Test3by3_2();
    void Test3by3_3();
    void Test3by3_4();
    void Test3by3_5();
    void Test4by4_1();
    void Test4by4_2();
    void Test4by4_3();
    void Test4by4_4();
};
