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
    CPPUNIT_TEST(RowIteratorDereferenceReturnsCorrectMatrixElements);
    CPPUNIT_TEST(RowIteratorPreIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(RowIteratorPostIncrementReturnsNextMatrixElements);
    CPPUNIT_TEST(RowIteratorCannotAdvancePastLastMatrixElement);
    CPPUNIT_TEST(RowIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull);
    CPPUNIT_TEST(IterateThroughAllElements);
    CPPUNIT_TEST(TestRowIteratorForMatrixWithOneElement);
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
    void RowIteratorDereferenceReturnsCorrectMatrixElements();
    void RowIteratorPreIncrementReturnsNextMatrixElements();
    void RowIteratorPostIncrementReturnsNextMatrixElements();
    void RowIteratorCannotAdvancePastLastMatrixElement();
    void RowIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull();
    void IterateThroughAllElements();
    void TestRowIteratorForMatrixWithOneElement();
};
