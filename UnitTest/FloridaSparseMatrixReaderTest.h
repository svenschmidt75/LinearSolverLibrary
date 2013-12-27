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
    CPPUNIT_TEST(symmetryStrategyTest);
    CPPUNIT_TEST_EXCEPTION(unknownSymmetryStrategyTest, std::runtime_error);

#ifdef INTEGRATION_TEST
    CPPUNIT_TEST(readSTS4098Test);
    CPPUNIT_TEST(readFS6801Test);
#endif
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void symmetryStrategyTest();
    void unknownSymmetryStrategyTest();
    void readSTS4098Test();
    void readFS6801Test();
};
