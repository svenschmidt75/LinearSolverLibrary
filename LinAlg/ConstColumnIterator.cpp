#include "pch.h"

#include "ConstColumnIterator.h"


namespace LinAlg_NS {


ConstColumnIterator<SparseMatrix2D>::ConstColumnIterator(SparseMatrix2D const & m, size_type row, size_type column)
    :
    m_(m), row_(row), column_(column) {}


} // namespace LinAlg_NS
