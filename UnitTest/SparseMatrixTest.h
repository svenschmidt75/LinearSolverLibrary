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
};
