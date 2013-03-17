/*
 * Name  : LinAlgTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/17/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class LinAlgTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinAlgTest);
    CPPUNIT_TEST(transposeSparseMatrixTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    // test methods
    void transposeSparseMatrixTest();
};
