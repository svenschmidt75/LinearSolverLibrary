/*
 * Name  : FloridaVectorReaderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 09/09/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class FloridaVectorReaderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FloridaVectorReaderTest);
    CPPUNIT_TEST(readSTS4098Test);
    CPPUNIT_TEST(readDeter7Test);
    CPPUNIT_TEST(readLF10Test);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void readSTS4098Test();
    void readDeter7Test();
    void readLF10Test();
};
