/*
* Name  : SparseMatrixIteratorTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class SparseMatrixIteratorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SparseMatrixIteratorTest);
    CPPUNIT_TEST(RowIteratorSizeEqualsNumberOfRows);
    CPPUNIT_TEST(RowIteratorReturnsCorrectRow);
    CPPUNIT_TEST(RowIteratorPostIncrementReturnsNextRowIterator);
    CPPUNIT_TEST(RowIteratorPostIncrementReturnsNextRowIterator);
    CPPUNIT_TEST(RowIteratorDereferenceReturnsRowIterator);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void RowIteratorSizeEqualsNumberOfRows();
    void RowIteratorReturnsCorrectRow();
    void RowIteratorPreIncrementReturnsNextRowIterator();
    void RowIteratorPostIncrementReturnsNextRowIterator();
    void RowIteratorDereferenceReturnsRowIterator();
};
