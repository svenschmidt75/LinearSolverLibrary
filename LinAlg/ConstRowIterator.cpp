#include "pch.h"

#include "ConstRowIterator.h"


namespace LinAlg_NS {

namespace iterator_internal {


ConstRowIterator<SparseMatrix2D>::ConstRowIterator(SparseMatrix2D const & m, size_type column, size_type row)
    : m_(m), column_(column), row_(row) {}

} // namespace iterator_internal

} // namespace LinAlg_NS
