/*
* Name  : ParallelLinAlgOperationsTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 12/07/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class ParallelLinAlgOperationsTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ParallelLinAlgOperationsTest);
//    CPPUNIT_TEST(TestParallelMatrixVectorMultiplication);
    CPPUNIT_TEST(TestParallelChunkedMatrixVectorMultiplication);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestParallelMatrixVectorMultiplication();
    void TestParallelChunkedMatrixVectorMultiplication();
};
