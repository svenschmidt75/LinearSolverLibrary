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
    CPPUNIT_TEST(Test2by2_1);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test2by2_1();
};
