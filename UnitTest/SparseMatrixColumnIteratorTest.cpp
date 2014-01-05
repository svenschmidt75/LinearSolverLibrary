#include "pch.h"

#include "SparseMatrixColumnIteratorTest.h"


using namespace LinAlg_NS;

namespace {

    SparseMatrix2D CreateSparseMatrix()
    {
        SparseMatrix2D m(3);
        m(0, 0) = 1;
        m(0, 2) = 3;
        m(1, 2) = 6;
        m(2, 0) = 7;
        m(2, 1) = 1;
        m.finalize();
        return m;
    }

}

void
SparseMatrixColumnIteratorTest::setUp() {}

void
SparseMatrixColumnIteratorTest::tearDown() {}

void
SparseMatrixColumnIteratorTest::RowIteratorSizeEqualsNumberOfRows() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", matrix.rows(), it.maxRows());
}

void
SparseMatrixColumnIteratorTest::GenericRowIteratorSizeEqualsNumberOfRows() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto expected = matrix2.rows();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of rows mismatch", expected, it.maxRows());
}

void
SparseMatrixColumnIteratorTest::RowIteratorReturnsCorrectRow() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current row mismatch", 0ull, it.row());

    it = MatrixIterators::getConstRowColumnIterator(matrix, 2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current row mismatch", 2ull, it.row());
}

void
SparseMatrixColumnIteratorTest::GenericRowIteratorReturnsCorrectRow() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current row mismatch", 0ull, it.row());

    it = MatrixIterators::getConstRowColumnIterator(matrix2, 2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Current row mismatch", 2ull, it.row());
}

void
SparseMatrixColumnIteratorTest::RowIteratorPreIncrementReturnsNextRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, (++it).row());
}

void
SparseMatrixColumnIteratorTest::GenericRowIteratorPreIncrementReturnsNextRowIterator() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto expected = 1ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", expected, (++it).row());
}

void
SparseMatrixColumnIteratorTest::RowIteratorPostIncrementReturnsNextRowIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    auto next_it = it++;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, next_it.row());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, it.row());
}

void
SparseMatrixColumnIteratorTest::GenericRowIteratorPostIncrementReturnsNextRowIterator() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto next_it = it++;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 0ull, next_it.row());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Row mismatch", 1ull, it.row());
}

void
SparseMatrixColumnIteratorTest::RowIteratorCannotAdvancePastLastRow() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !it.isValid());
}

void
SparseMatrixColumnIteratorTest::GenericRowIteratorCannotAdvancePastLastRow() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());
    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !it.isValid());
}

void
SparseMatrixColumnIteratorTest::RowIteratorDereferenceReturnsColumnIterator() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
}

void
SparseMatrixColumnIteratorTest::GenericRowIteratorDereferenceReturnsColumnIterator() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto rowit = *it;
}

void
SparseMatrixColumnIteratorTest::RowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of non-zero elements mismatch", 2ull, it.numberOfNonZeroMatrixElements());
}

void
SparseMatrixColumnIteratorTest::GenericRowIteratorReturnsNumberOfNonZeroMatrixElementsInCurrentRow() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto expected = 3ull;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of non-zero elements mismatch", expected, it.numberOfNonZeroMatrixElements());
}

namespace {

    bool almostEqual(double d1, double d2, double tol = 1E-3) {
        if (d1 == 0.0)
            return true;
        double diff = std::fabs((d1 - d2) / d1);
        bool success = diff < tol;
        return success;
    }

}

void
SparseMatrixColumnIteratorTest::ColumnIteratorDereferenceReturnsCorrectMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *rowit));
}

void
SparseMatrixColumnIteratorTest::GenericColumnIteratorDereferenceReturnsCorrectMatrixElements() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(22.0, *rowit));
}

void
SparseMatrixColumnIteratorTest::ColumnIteratorPreIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    ++rowit;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, rowit.column());
}

void
SparseMatrixColumnIteratorTest::GenericColumnIteratorPreIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto rowit = *it;
    ++rowit;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, rowit.column());
}

void
SparseMatrixColumnIteratorTest::ColumnIteratorPostIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, rowit.column());
    auto val = *rowit++;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, val));
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, rowit.column());
}

void
SparseMatrixColumnIteratorTest::GenericColumnIteratorPostIncrementReturnsNextMatrixElements() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, rowit.column());
    auto val = *rowit++;
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(22.0, val));
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, rowit.column());
}

void
SparseMatrixColumnIteratorTest::ColumnIteratorCannotAdvancePastLastMatrixElement() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !rowit.isValid());
}

void
SparseMatrixColumnIteratorTest::GenericColumnIteratorCannotAdvancePastLastMatrixElement() {
    auto matrix = CreateSparseMatrix();
    auto matrix2 =  matrix * matrix;
    /*
         22        3        3
         42        6        0
          7        0       27
     */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !rowit.isValid());

    it = MatrixIterators::getConstRowColumnIterator(matrix2, 1);
    rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !rowit.isValid());

    it = MatrixIterators::getConstRowColumnIterator(matrix2, 2);
    rowit = *it;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", rowit.isValid());
    rowit++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !rowit.isValid());
}

void
SparseMatrixColumnIteratorTest::ColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull() {
    SparseMatrix2D matrix(3);
    matrix(0, 2) = 3;
    matrix(1, 2) = 6;
    matrix(2, 0) = 7;
    matrix.finalize();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, rowit.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
}

void
SparseMatrixColumnIteratorTest::GenericColumnIteratorDereferenceReturnsCorrectMatrixElementsIf1stOneNull() {
    SparseMatrix2D matrixA{3, 3};
    /*
        1.000 0.000 3.000
        0.000 0.000 6.000
        7.000 1.000 0.000
    */
    matrixA(0, 0) = 1;
    matrixA(0, 2) = 3;
    matrixA(1, 2) = 6;
    matrixA(2, 0) = 7;
    matrixA(2, 1) = 1;
    matrixA.finalize();

    SparseMatrix2D matrixB{3, 3};
    /*
        0.000 0.000 3.000
        1.000 0.000 6.000
        0.000 1.000 0.000
    */
    matrixB(0, 2) = 3;
    matrixB(1, 0) = 1;
    matrixB(1, 2) = 6;
    matrixB(2, 1) = 1;
    matrixB.finalize();

    auto matrix2 = matrixA * matrixB;
    /*
        0.000  3.000  3.000
        0.000  6.000  0.000
        1.000  0.000 27.000
    */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto rowit = *it;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, rowit.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *rowit));
}

void
SparseMatrixColumnIteratorTest::IterateThroughAllElements() {
    auto matrix = CreateSparseMatrix();
    ConstRowColumnIterator<SparseMatrix2D> it = MatrixIterators::getConstRowColumnIterator(matrix);
    ConstColumnIterator<SparseMatrix2D> column_it = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 1
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    column_it = *++it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 2
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(6.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    it++;
    column_it = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 3
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(7.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !it.isValid());
}

void
SparseMatrixColumnIteratorTest::GenericIterateThroughAllElements() {
    auto matrix = CreateSparseMatrix();
    SparseMatrix2D matrixA{3, 3};
    /*
        1.000 0.000 3.000
        0.000 0.000 6.000
        7.000 1.000 0.000
    */
    matrixA(0, 0) = 1;
    matrixA(0, 2) = 3;
    matrixA(1, 2) = 6;
    matrixA(2, 0) = 7;
    matrixA(2, 1) = 1;
    matrixA.finalize();

    SparseMatrix2D matrixB{3, 3};
    /*
        0.000 0.000 3.000
        1.000 0.000 6.000
        0.000 1.000 0.000
    */
    matrixB(0, 2) = 3;
    matrixB(1, 0) = 1;
    matrixB(1, 2) = 6;
    matrixB(2, 1) = 1;
    matrixB.finalize();

    auto matrix2 = matrixA * matrixB;
    /*
        0.000  3.000  3.000
        0.000  6.000  0.000
        1.000  0.000 27.000
    */
    auto it = MatrixIterators::getConstRowColumnIterator(matrix2);
    auto column_it = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 1
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(3.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    column_it = *++it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 2
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 1ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(6.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    it++;
    column_it = *it;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be valid", it.isValid());

    // row 3
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 0ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(1.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be valid", column_it.isValid());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Column mismatch", 2ull, column_it.column());
    CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(27.0, *column_it));
    column_it++;
    CPPUNIT_ASSERT_MESSAGE("Column iterator should be invalid", !column_it.isValid());

    it++;
    CPPUNIT_ASSERT_MESSAGE("Row iterator should be invalid", !it.isValid());
}
