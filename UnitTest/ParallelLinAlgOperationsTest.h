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
    CPPUNIT_TEST(TestNonChunkedParallelMatrixVectorMultiplication);
    CPPUNIT_TEST(TestChunkedParallelMatrixVectorMultiplication);
    CPPUNIT_TEST(TestNonChunkedParallelDotProduct);
    CPPUNIT_TEST(TestChunkedParallelDotProduct);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestNonChunkedParallelMatrixVectorMultiplication();
    void TestChunkedParallelMatrixVectorMultiplication();
    void TestNonChunkedParallelDotProduct();
    void TestChunkedParallelDotProduct();
};
