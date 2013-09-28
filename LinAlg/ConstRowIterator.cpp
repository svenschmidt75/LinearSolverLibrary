#include "pch.h"

#include "ConstRowIterator.h"
#include "ConstColumnIterator.h"


namespace LinAlg_NS {


ConstRowIterator<SparseMatrix2D>::ConstRowIterator(SparseMatrix2D const & m)
    :
    m_(m), row_(0) {}

ConstRowIterator<SparseMatrix2D>::ConstRowIterator(SparseMatrix2D const & m, size_type row)
    :
    m_(m), row_(row) {}

ConstRowIterator<SparseMatrix2D>::size_type
ConstRowIterator<SparseMatrix2D>::maxColumns() const {
    return m_.cols();
}

ConstRowIterator<SparseMatrix2D>::size_type
ConstRowIterator<SparseMatrix2D>::column() const {
    return row_;
}

ConstRowIterator<SparseMatrix2D> const &
ConstRowIterator<SparseMatrix2D>::operator++() const {
    row_++;
    return *this;
}

ConstRowIterator<SparseMatrix2D>
ConstRowIterator<SparseMatrix2D>::operator++(int) const {
    iter tmp(*this);
    row_++;
    return tmp;
}

ConstColumnIterator<SparseMatrix2D>
ConstRowIterator<SparseMatrix2D>::operator*() const {
    return ConstColumnIterator<SparseMatrix2D>(m_, row_, 0);
}


} // namespace LinAlg_NS
