/*
 * Name  : FloridaSparseMatrixReaderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 08/12/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class FloridaSparseMatrixReaderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FloridaSparseMatrixReaderTest);
    CPPUNIT_TEST(readTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void readTest();
};