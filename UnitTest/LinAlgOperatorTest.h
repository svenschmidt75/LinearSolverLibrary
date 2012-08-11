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
    CPPUNIT_TEST(scalarVectorExprMulTest);
    CPPUNIT_TEST(vectorExprAddTest);
    CPPUNIT_TEST(scalarMatrixMulTest);
    CPPUNIT_TEST(scalarSparseMatrixMulTest);
    CPPUNIT_TEST(sparseMatrixVectorMulTest);
    CPPUNIT_TEST(matrixVectorMulTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void scalarVectorMulTest();
    void scalarVectorExprMulTest();
    void vectorExprAddTest();
    void scalarMatrixMulTest();
    void scalarSparseMatrixMulTest();
    void sparseMatrixVectorMulTest();
    void matrixVectorMulTest();
};
