/*
 * Name  : UTMatrixTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/07/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class UTMatrixTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(UTMatrixTest);
    CPPUNIT_TEST(indexTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    // test methods
    void indexTest();
};
