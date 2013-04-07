/*
 * Name  : UHMatrixTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/07/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class UHMatrixTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(UHMatrixTest);
    CPPUNIT_TEST(indexTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    // test methods
    void indexTest();
};
