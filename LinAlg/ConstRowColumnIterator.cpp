#include "pch.h"

#include "ConstRowColumnIterator.h"
#include "ConstColumnIterator.h"

#include "common/reporting.h"


namespace LinAlg_NS {


ConstRowColumnIterator<SparseMatrix2D>::ConstRowColumnIterator(SparseMatrix2D const & m)
    :
    ConstRowColumnIterator(m, 0) {}

ConstRowColumnIterator<SparseMatrix2D>::ConstRowColumnIterator(SparseMatrix2D const & m, size_type row)
    :
    m_(m),
    row_(row) {}

ConstRowColumnIterator<SparseMatrix2D> &
ConstRowColumnIterator<SparseMatrix2D>::operator=(ConstRowColumnIterator const & in) {
    const_cast<SparseMatrix2D &>(m_) = in.m_;
    row_                             = in.row_;
    return *this;
}

ConstRowColumnIterator<SparseMatrix2D>::operator bool() const {
    return isValid();
}

bool 
ConstRowColumnIterator<SparseMatrix2D>::isValid() const {
    common_NS::reporting::checkUppderBound(row_, m_.rows());
    return row_ < maxRows();
}

ConstRowColumnIterator<SparseMatrix2D>::size_type
ConstRowColumnIterator<SparseMatrix2D>::maxRows() const {
    return m_.rows();
}

ConstRowColumnIterator<SparseMatrix2D>::size_type
ConstRowColumnIterator<SparseMatrix2D>::row() const {
    return row_;
}

ConstRowColumnIterator<SparseMatrix2D>::size_type
ConstRowColumnIterator<SparseMatrix2D>::numberOfNonZeroMatrixElements() const {
    return (*(*this)).numberOfNonZeroMatrixElements();
}

ConstRowColumnIterator<SparseMatrix2D> &
ConstRowColumnIterator<SparseMatrix2D>::operator++() {
    row_++;
    return *this;
}

ConstRowColumnIterator<SparseMatrix2D>
ConstRowColumnIterator<SparseMatrix2D>::operator++(int) {
    iter tmp(*this);
    row_++;
    return tmp;
}

ConstColumnIterator<SparseMatrix2D>
ConstRowColumnIterator<SparseMatrix2D>::operator*() const {
    return ConstColumnIterator<SparseMatrix2D>(m_, row_);
}


} // namespace LinAlg_NS
