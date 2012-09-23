/*
 * Name  : MultiColorTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class MultiColorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MultiColorTest);
    CPPUNIT_TEST(simpleTest);
    CPPUNIT_TEST(bucketElementTest);
    CPPUNIT_TEST(bucketListDecompositionTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void bucketListDecompositionTest();
    void bucketElementTest();
    void simpleTest();
};
