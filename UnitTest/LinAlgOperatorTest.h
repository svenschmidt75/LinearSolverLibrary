/*
 * Name  : LinAlgOperatorTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/29/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class LinAlgOperatorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinAlgOperatorTest);
    CPPUNIT_TEST(scalarVectorMulTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void scalarVectorMulTest();
};
