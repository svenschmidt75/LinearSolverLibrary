/*
* Name  : SparseMatrixTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 11/30/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class SparseMatrixTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SparseMatrixTest);
    CPPUNIT_TEST(testCreateSquareSparseMatrix);
    CPPUNIT_TEST(testCreateNonSquareSparseMatrix);
    CPPUNIT_TEST(testAccessToNonExistingElementReturnsNull);
    CPPUNIT_TEST(sparseSquareMatrixTranspose);
    CPPUNIT_TEST(sparseNonSquareMatrixTranspose);
    CPPUNIT_TEST(testColumnIndicesFor5by5SparseMatrix);
    CPPUNIT_TEST(testRowIndicesFor5by5SparseMatrix);
    CPPUNIT_TEST(testColumnIndicesFor3by5SparseMatrix);
    CPPUNIT_TEST(testRowIndicesFor3by5SparseMatrix);
    CPPUNIT_TEST(testColumnIndicesForSquareMatrixMatrixMul);
    CPPUNIT_TEST(testRowIndicesForSquareMatrixMatrixMul);
    CPPUNIT_TEST(testColumnIndicesForNonSquareMatrixMatrixMul);
    CPPUNIT_TEST(testRowIndicesForNonSquareMatrixMatrixMul);
    CPPUNIT_TEST_SUITE_END();

public:
    void SparseMatrixRowIteratorTest();
    void tearDown();

protected:
    void testCreateSquareSparseMatrix();
    void testCreateNonSquareSparseMatrix();
    void testAccessToNonExistingElementReturnsNull();
    void sparseSquareMatrixTranspose();
    void sparseNonSquareMatrixTranspose();
    void testColumnIndicesFor5by5SparseMatrix();
    void testRowIndicesFor5by5SparseMatrix();
    void testColumnIndicesFor3by5SparseMatrix();
    void testRowIndicesFor3by5SparseMatrix();
    void testColumnIndicesForSquareMatrixMatrixMul();
    void testRowIndicesForSquareMatrixMatrixMul();
    void testColumnIndicesForNonSquareMatrixMatrixMul();
    void testRowIndicesForNonSquareMatrixMatrixMul();
};
