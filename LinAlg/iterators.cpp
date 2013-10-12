#include "pch.h"

#include "iterators.h"
#include "ConstColumnIterator.h"

namespace LinAlg_NS {


ConstRowIterator<SparseMatrix2D>
iterators::getConstRowIterator(SparseMatrix2D const & matrix) {
    return ConstRowIterator<SparseMatrix2D>(matrix);
}

ConstRowIterator<SparseMatrix2D>
iterators::getConstRowIterator(SparseMatrix2D const & matrix, IMatrix2D::size_type row) {
    return ConstRowIterator<SparseMatrix2D>(matrix, row);
}


} // namespace LinAlg_NS
