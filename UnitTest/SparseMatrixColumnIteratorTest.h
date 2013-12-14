/*
* Name  : SparseMatrixColumnIteratorTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class SparseMatrixColumnIteratorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SparseMatrixColumnIteratorTest);
    CPPUNIT_TEST(RowIteratorSizeEqualsNumberOfRows);
    CPPUNIT_TEST(RowIteratorReturnsCorrectRow);
    CPPUNIT_TEST(RowIteratorPreIncrementReturnsNextRowIterator);
    CPPUNIT_TEST(RowIteratorPostIncrementReturnsNextRowIterator);
    CPPUNIT_TEST(RowIteratorCannotAdvancePastLastRow);
    CPPUNIT_TEST(RowIteratorDereferenceReturnsColumnIterator);
    CPPUNIT_TEST(RowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow);
    CPPUNIT_TEST(ColumnIteratorDereferenceReturnsCorrectMatrixElements);
    CPPUNIT_TEST(ColumnIteratorPreIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(ColumnIteratorPostIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(ColumnIteratorCannotAdvancePastLastMatrixElement);
    CPPUNIT_TEST(ColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull);
    CPPUNIT_TEST(IterateThroughAllElements);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void RowIteratorSizeEqualsNumberOfRows();
    void RowIteratorReturnsCorrectRow();
    void RowIteratorPreIncrementReturnsNextRowIterator();
    void RowIteratorPostIncrementReturnsNextRowIterator();
    void RowIteratorCannotAdvancePastLastRow();
    void RowIteratorDereferenceReturnsColumnIterator();
    void RowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow();
    void ColumnIteratorDereferenceReturnsCorrectMatrixElements();
    void ColumnIteratorPreIncrementReturnsNextMatrixElements();
    void ColumnIteratorPostIncrementReturnsNextMatrixElements();
    void ColumnIteratorCannotAdvancePastLastMatrixElement();
    void ColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull();
    void IterateThroughAllElements();
};