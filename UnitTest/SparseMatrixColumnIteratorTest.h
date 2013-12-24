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
    CPPUNIT_TEST(GenericRowIteratorSizeEqualsNumberOfRows);
    CPPUNIT_TEST(RowIteratorReturnsCorrectRow);
    CPPUNIT_TEST(GenericRowIteratorReturnsCorrectRow);
    CPPUNIT_TEST(RowIteratorPreIncrementReturnsNextRowIterator);
    CPPUNIT_TEST(GenericRowIteratorPreIncrementReturnsNextRowIterator);
    CPPUNIT_TEST(RowIteratorPostIncrementReturnsNextRowIterator);
    CPPUNIT_TEST(GenericRowIteratorPostIncrementReturnsNextRowIterator);
    CPPUNIT_TEST(RowIteratorCannotAdvancePastLastRow);
    CPPUNIT_TEST(GenericRowIteratorCannotAdvancePastLastRow);
    CPPUNIT_TEST(RowIteratorDereferenceReturnsColumnIterator);
    CPPUNIT_TEST(GenericRowIteratorDereferenceReturnsColumnIterator);
    CPPUNIT_TEST(RowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow);
    CPPUNIT_TEST(GenericRowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow);
    CPPUNIT_TEST(ColumnIteratorDereferenceReturnsCorrectMatrixElements);
    CPPUNIT_TEST(GenericColumnIteratorDereferenceReturnsCorrectMatrixElements);
    CPPUNIT_TEST(ColumnIteratorPreIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(GenericColumnIteratorPreIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(ColumnIteratorPostIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(GenericColumnIteratorPostIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(ColumnIteratorCannotAdvancePastLastMatrixElement);
    CPPUNIT_TEST(GenericColumnIteratorCannotAdvancePastLastMatrixElement);
    CPPUNIT_TEST(ColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull);
    CPPUNIT_TEST(GenericColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull);
    CPPUNIT_TEST(IterateThroughAllElements);
    CPPUNIT_TEST(GenericIterateThroughAllElements);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void RowIteratorSizeEqualsNumberOfRows();
    void GenericRowIteratorSizeEqualsNumberOfRows();
    void RowIteratorReturnsCorrectRow();
    void GenericRowIteratorReturnsCorrectRow();
    void RowIteratorPreIncrementReturnsNextRowIterator();
    void GenericRowIteratorPreIncrementReturnsNextRowIterator();
    void RowIteratorPostIncrementReturnsNextRowIterator();
    void GenericRowIteratorPostIncrementReturnsNextRowIterator();
    void RowIteratorCannotAdvancePastLastRow();
    void GenericRowIteratorCannotAdvancePastLastRow();
    void RowIteratorDereferenceReturnsColumnIterator();
    void GenericRowIteratorDereferenceReturnsColumnIterator();
    void RowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow();
    void GenericRowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow();
    void ColumnIteratorDereferenceReturnsCorrectMatrixElements();
    void GenericColumnIteratorDereferenceReturnsCorrectMatrixElements();
    void ColumnIteratorPreIncrementReturnsNextMatrixElements();
    void GenericColumnIteratorPreIncrementReturnsNextMatrixElements();
    void ColumnIteratorPostIncrementReturnsNextMatrixElements();
    void GenericColumnIteratorPostIncrementReturnsNextMatrixElements();
    void ColumnIteratorCannotAdvancePastLastMatrixElement();
    void GenericColumnIteratorCannotAdvancePastLastMatrixElement();
    void ColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull();
    void GenericColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull();
    void IterateThroughAllElements();
    void GenericIterateThroughAllElements();
};
