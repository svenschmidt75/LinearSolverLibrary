/*
* Name  : LinAlgOperatorTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 11/28/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class LinAlgOperatorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinAlgOperatorTest);
    CPPUNIT_TEST(vectorMulScalarTest);
    CPPUNIT_TEST(vectorAddScalarMulVectorTest);
    CPPUNIT_TEST(vectorMulEqualScalarTest);
    CPPUNIT_TEST(vectorAddVectorTest);
    CPPUNIT_TEST(vectorSubVectorTest);
    CPPUNIT_TEST(vectorExprMulScalarTest);
    CPPUNIT_TEST(vectorPlusEqualVectorTest);
    CPPUNIT_TEST(vectorPlusEqualVectorScalarExprTest);
    CPPUNIT_TEST(vectorPlusEqualVectorMulTest);
    CPPUNIT_TEST(addScalarMulVectorTest);
    CPPUNIT_TEST(vectorPlusEqualVectorBinaryExprTest);
    CPPUNIT_TEST(vectorExprDivScalarTest);
    CPPUNIT_TEST(vectorExprAddVectorExprTest);
    CPPUNIT_TEST(vectorExprAddVectorScalarExprTest);
    CPPUNIT_TEST(vectorMinusEqualVectorTest);
    CPPUNIT_TEST(complexVectorAddExprTest);
    CPPUNIT_TEST(scalarMatrixMulTest);
    CPPUNIT_TEST(scalarSparseMatrixMulTest);
    CPPUNIT_TEST(matrixVectorMulTest);
    CPPUNIT_TEST(sparseMatrixVectorMulTest);
    CPPUNIT_TEST(matrixVectorExprMulTest);
    CPPUNIT_TEST(sparseMatrixVectorExprMulTest);
    CPPUNIT_TEST(sparseSquareMatrixMulSparseSquareMatrixTest);
    CPPUNIT_TEST(sparseSquareMatrixMulSparseNonSquareMatrixTest);
    CPPUNIT_TEST(test3by3MatrixTimes3by3MatrixEqualsIdentity);
    CPPUNIT_TEST(test3x5x5MatrixMulVector);
    CPPUNIT_TEST(test3x25x25MatrixMulVector);
    CPPUNIT_TEST_SUITE_END();

public:
    void SparseMatrixRowIteratorTest();
    void tearDown();

protected:
    void vectorMulScalarTest();
    void vectorAddScalarMulVectorTest();
    void vectorMulEqualScalarTest();
    void vectorAddVectorTest();
    void vectorSubVectorTest();
    void vectorExprMulScalarTest();
    void vectorPlusEqualVectorTest();
    void vectorPlusEqualVectorScalarExprTest();
    void vectorPlusEqualVectorMulTest();
    void addScalarMulVectorTest();
    void vectorPlusEqualVectorBinaryExprTest();
    void vectorExprDivScalarTest();
    void vectorExprAddVectorExprTest();
    void vectorExprAddVectorScalarExprTest();
    void vectorMinusEqualVectorTest();
    void complexVectorAddExprTest();
    void scalarMatrixMulTest();
    void scalarSparseMatrixMulTest();
    void matrixVectorMulTest();
    void sparseMatrixVectorMulTest();
    void matrixVectorExprMulTest();
    void sparseMatrixVectorExprMulTest();
    void sparseSquareMatrixMulSparseSquareMatrixTest();
    void sparseSquareMatrixMulSparseNonSquareMatrixTest();
    void test3by3MatrixTimes3by3MatrixEqualsIdentity();
    void test3x5x5MatrixMulVector();
    void test3x25x25MatrixMulVector();
};
