/*
 * Name  : LUDecompositionTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 08/11/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "LinAlg/entities.h"


class LUDecompositionTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LUDecompositionTest);
    CPPUNIT_TEST(Test3by3_1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test3by3_1();
};
