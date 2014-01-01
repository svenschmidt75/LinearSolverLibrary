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
    CPPUNIT_TEST(testChunkGenerationAlgorithmUneven);
    CPPUNIT_TEST(testChunkGenerationAlgorithmEven);
    CPPUNIT_TEST(testNonChunkedParallelMatrixVectorMultiplication);
    CPPUNIT_TEST(testChunkedParallelMatrixVectorMultiplication);
#ifdef INTEGRATION_TEST
    CPPUNIT_TEST(testParallelLargeMatrixVectorMultiplication); // Matrix multiplication takes way too long
#endif
    CPPUNIT_TEST(testNonChunkedParallelDotProduct);
    CPPUNIT_TEST(testChunkedParallelDotProduct);
    CPPUNIT_TEST(testNonChunkedParallelMatrixProduct);
    CPPUNIT_TEST(testChunkedParallelMatrixProduct);
    CPPUNIT_TEST(testNonChunkedParallelNonSquareMatrixProduct);
    CPPUNIT_TEST(testChunkedParallelNonSquareMatrixProduct);
    CPPUNIT_TEST(testNonChunkedMatrixIsSymmetric);
    CPPUNIT_TEST(testChunkedMatrixIsSymmetric);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testChunkGenerationAlgorithmUneven();
    void testChunkGenerationAlgorithmEven();
    void testNonChunkedParallelMatrixVectorMultiplication();
    void testChunkedParallelMatrixVectorMultiplication();
    void testParallelLargeMatrixVectorMultiplication();
    void testNonChunkedParallelDotProduct();
    void testChunkedParallelDotProduct();
    void testNonChunkedParallelMatrixProduct();
    void testChunkedParallelMatrixProduct();
    void testNonChunkedParallelNonSquareMatrixProduct();
    void testChunkedParallelNonSquareMatrixProduct();
    void testNonChunkedMatrixIsSymmetric();
    void testChunkedMatrixIsSymmetric();
};
