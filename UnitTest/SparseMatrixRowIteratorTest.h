/*
* Name  : SparseMatrixRowIteratorTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 11/29/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class SparseMatrixRowIteratorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SparseMatrixRowIteratorTest);
    CPPUNIT_TEST(ColumnIteratorSizeEqualsNumberOfColumns);
    CPPUNIT_TEST(GenericColumnIteratorSizeEqualsNumberOfColumns);
    CPPUNIT_TEST(ColumnIteratorReturnsCorrectColumn);
    CPPUNIT_TEST(GenericColumnIteratorReturnsCorrectColumn);
    CPPUNIT_TEST(ColumnIteratorPreIncrementReturnsNextColumnIterator);
    CPPUNIT_TEST(GenericColumnIteratorPreIncrementReturnsNextColumnIterator);
    CPPUNIT_TEST(ColumnIteratorPostIncrementReturnsNextColumnIterator);
    CPPUNIT_TEST(GenericColumnIteratorPostIncrementReturnsNextColumnIterator);
    CPPUNIT_TEST(ColumnIteratorCannotAdvancePastLastColumn);
    CPPUNIT_TEST(GenericColumnIteratorCannotAdvancePastLastColumn);
    CPPUNIT_TEST(ColumnIteratorDereferenceReturnsRowIterator);
    CPPUNIT_TEST(GenericColumnIteratorDereferenceReturnsRowIterator);
    CPPUNIT_TEST(ColumnIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentColumn);
    CPPUNIT_TEST(GenericColumnIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentColumn);
    CPPUNIT_TEST(RowIteratorInitializesToFirstElement);
    CPPUNIT_TEST(GenericRowIteratorInitializesToFirstElement);
    CPPUNIT_TEST(RowIteratorDereferenceReturnsCorrectMatrixElements);
    CPPUNIT_TEST(GenericRowIteratorDereferenceReturnsCorrectMatrixElements);
    CPPUNIT_TEST(RowIteratorPreIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(GenericRowIteratorPreIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(RowIteratorPostIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(GenericRowIteratorPostIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(RowIteratorCannotAdvancePastLastMatrixElement);
    CPPUNIT_TEST(GenericRowIteratorCannotAdvancePastLastMatrixElement);
    CPPUNIT_TEST(RowIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull);
    CPPUNIT_TEST(GenericRowIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull);
    CPPUNIT_TEST(IterateThroughAllElements);
    CPPUNIT_TEST(GenericIterateThroughAllElements);
    CPPUNIT_TEST(TestRowIteratorForMatrixWithOneElement);
    CPPUNIT_TEST(GenericTestRowIteratorForMatrixWithOneElement);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void ColumnIteratorSizeEqualsNumberOfColumns();
    void GenericColumnIteratorSizeEqualsNumberOfColumns();
    void ColumnIteratorReturnsCorrectColumn();
    void GenericColumnIteratorReturnsCorrectColumn();
    void ColumnIteratorPreIncrementReturnsNextColumnIterator();
    void GenericColumnIteratorPreIncrementReturnsNextColumnIterator();
    void ColumnIteratorPostIncrementReturnsNextColumnIterator();
    void GenericColumnIteratorPostIncrementReturnsNextColumnIterator();
    void ColumnIteratorCannotAdvancePastLastColumn();
    void GenericColumnIteratorCannotAdvancePastLastColumn();
    void ColumnIteratorDereferenceReturnsRowIterator();
    void GenericColumnIteratorDereferenceReturnsRowIterator();
    void ColumnIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentColumn();
    void GenericColumnIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentColumn();
    void RowIteratorInitializesToFirstElement();
    void GenericRowIteratorInitializesToFirstElement();
    void RowIteratorDereferenceReturnsCorrectMatrixElements();
    void GenericRowIteratorDereferenceReturnsCorrectMatrixElements();
    void RowIteratorPreIncrementReturnsNextMatrixElements();
    void GenericRowIteratorPreIncrementReturnsNextMatrixElements();
    void RowIteratorPostIncrementReturnsNextMatrixElements();
    void GenericRowIteratorPostIncrementReturnsNextMatrixElements();
    void RowIteratorCannotAdvancePastLastMatrixElement();
    void GenericRowIteratorCannotAdvancePastLastMatrixElement();
    void RowIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull();
    void GenericRowIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull();
    void IterateThroughAllElements();
    void GenericIterateThroughAllElements();
    void TestRowIteratorForMatrixWithOneElement();
    void GenericTestRowIteratorForMatrixWithOneElement();
};
