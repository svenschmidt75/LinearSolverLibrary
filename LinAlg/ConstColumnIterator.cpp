#include "pch.h"

#include "ConstColumnIterator.h"
#include "ConstRowIterator.h"


namespace LinAlg_NS {

namespace iterator_internal {

ConstColumnIterator<SparseMatrix2D>::ConstColumnIterator(SparseMatrix2D const & m)
    :
    m_(m),
    current_column_(0) {}

ConstColumnIterator<SparseMatrix2D>::ConstColumnIterator(SparseMatrix2D const & m, size_type startColumn)
    :
    m_(m),
    current_column_(startColumn) {}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::maxColumns() const {
    return m_.cols();
}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::column() const {
    return current_column_;
}

ConstColumnIterator<SparseMatrix2D> const &
ConstColumnIterator<SparseMatrix2D>::operator++() const {
    current_column_++;
    return *this;
}

ConstColumnIterator<SparseMatrix2D>
ConstColumnIterator<SparseMatrix2D>::operator++(int) const {
    iter tmp(*this);
    current_column_++;
    return tmp;
}

ConstRowIterator<SparseMatrix2D>
ConstColumnIterator<SparseMatrix2D>::operator*() const {
    return ConstRowIterator<SparseMatrix2D>(m_, current_column_, 0);
}

} // namespace iterator_internal

} // namespace LinAlg_NS
