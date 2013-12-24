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
    CPPUNIT_TEST(ColumnIteratorPostIncrementReturnsNextColumnIterator);
    CPPUNIT_TEST(ColumnIteratorCannotAdvancePastLastColumn);
    CPPUNIT_TEST(ColumnIteratorDereferenceReturnsRowIterator);
    CPPUNIT_TEST(ColumnIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentColumn);
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
    void ColumnIteratorPostIncrementReturnsNextColumnIterator();
    void ColumnIteratorCannotAdvancePastLastColumn();
    void ColumnIteratorDereferenceReturnsRowIterator();
    void ColumnIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentColumn();
    void RowIteratorInitializesToFirstElement();
    void RowIteratorDereferenceReturnsCorrectMatrixElements();
    void RowIteratorPreIncrementReturnsNextMatrixElements();
    void RowIteratorPostIncrementReturnsNextMatrixElements();
    void RowIteratorCannotAdvancePastLastMatrixElement();
    void RowIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull();
    void IterateThroughAllElements();
    void TestRowIteratorForMatrixWithOneElement();
};
