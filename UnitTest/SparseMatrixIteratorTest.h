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
    CPPUNIT_TEST(ColumnIteratorSizeEqualsNumberOfColumns);
    CPPUNIT_TEST(ColumnIteratorReturnsCorrectColumn);
    CPPUNIT_TEST(ColumnIteratorPostIncrementReturnsNextColumnIterator);
    CPPUNIT_TEST(ColumnIteratorPostIncrementReturnsNextColumnIterator);
    CPPUNIT_TEST(ColumnIteratorDereferenceReturnsRowIterator);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void ColumnIteratorSizeEqualsNumberOfColumns();
    void ColumnIteratorReturnsCorrectColumn();
    void ColumnIteratorPreIncrementReturnsNextColumnIterator();
    void ColumnIteratorPostIncrementReturnsNextColumnIterator();
    void ColumnIteratorDereferenceReturnsRowIterator();
};