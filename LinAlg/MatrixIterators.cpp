#include "pch.h"

#include "MatrixIterators.h"
#include "ConstRowColumnIterator.h"
#include "ConstColumnRowIterator.h"


namespace LinAlg_NS {


ConstRowColumnIterator<SparseMatrix2D>
MatrixIterators::getConstRowColumnIterator(SparseMatrix2D const & matrix) {
    return ConstRowColumnIterator<SparseMatrix2D>(matrix);
}

ConstRowColumnIterator<SparseMatrix2D>
MatrixIterators::getConstRowColumnIterator(SparseMatrix2D const & matrix, IMatrix2D::size_type row) {
    return ConstRowColumnIterator<SparseMatrix2D>(matrix, row);
}

ConstColumnRowIterator<SparseMatrix2D>
MatrixIterators::getConstColumnRowIterator(SparseMatrix2D const & matrix) {
    return ConstColumnRowIterator<SparseMatrix2D>(matrix);
}

ConstColumnRowIterator<SparseMatrix2D>
MatrixIterators::getConstColumnRowIterator(SparseMatrix2D const & matrix, IMatrix2D::size_type column) {
    return ConstColumnRowIterator<SparseMatrix2D>(matrix, column);
}

} // namespace LinAlg_NS
