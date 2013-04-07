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
    CPPUNIT_TEST(vectorAddScalarMulVectorTest);
    CPPUNIT_TEST(addScalarMulVectorTest);
    CPPUNIT_TEST(addVectorTest);
    CPPUNIT_TEST(subVectorTest);
    CPPUNIT_TEST(scalarVectorMulTest);
    CPPUNIT_TEST(scalarVectorExprMulTest);
    CPPUNIT_TEST(vectorExprAddTest);
    CPPUNIT_TEST(vectorAddVectorMulTest);
    CPPUNIT_TEST(scalarMatrixMulTest);
    CPPUNIT_TEST(scalarSparseMatrixMulTest);
    CPPUNIT_TEST(sparseMatrixVectorMulTest);
    CPPUNIT_TEST(matrixVectorMulTest);
    CPPUNIT_TEST(matrixVectorExprMulTest);
    CPPUNIT_TEST(sparseMatrixVectorExprMulTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void vectorAddScalarMulVectorTest();
    void addScalarMulVectorTest();
    void addVectorTest();
    void subVectorTest();
    void scalarVectorMulTest();
    void scalarVectorExprMulTest();
    void vectorExprAddTest();
    void vectorAddVectorMulTest();
    void scalarMatrixMulTest();
    void scalarSparseMatrixMulTest();
    void sparseMatrixVectorMulTest();
    void matrixVectorMulTest();
    void matrixVectorExprMulTest();
    void sparseMatrixVectorExprMulTest();
};
