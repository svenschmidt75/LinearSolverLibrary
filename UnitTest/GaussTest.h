/*
 * Name  : GaussTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 08/03/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "LinAlg/entities.h"


class GaussTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(GaussTest);
//    CPPUNIT_TEST(Test2by2);
    CPPUNIT_TEST(Test4by4);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void Test2by2();
    void Test4by4();
};
