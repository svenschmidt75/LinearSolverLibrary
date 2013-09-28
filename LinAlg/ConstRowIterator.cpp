#include "pch.h"

#include "ConstRowIterator.h"


namespace LinAlg_NS {


ConstRowIterator<SparseMatrix2D>::ConstRowIterator(SparseMatrix2D const & m, size_type column, size_type row)
    : m_(m), column_(column), row_(row) {}


} // namespace LinAlg_NS
