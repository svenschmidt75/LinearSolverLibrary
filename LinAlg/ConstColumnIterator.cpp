#include "pch.h"

#include "ConstColumnIterator.h"
#include "common/reporting.h"


namespace LinAlg_NS {


ConstColumnIterator<SparseMatrix2D>::ConstColumnIterator(SparseMatrix2D const & m, size_type row)
    :
    m_(m), row_(row) {

#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
    jumpToFirstElement();
}

ConstColumnIterator<SparseMatrix2D> &
ConstColumnIterator<SparseMatrix2D>::operator=(ConstColumnIterator const & in) {
    const_cast<SparseMatrix2D &>(m_) = in.m_;
    column_                          = in.column_;
    column_mapped_                   = in.column_mapped_;
    row_                             = in.row_;
    return *this;
}

ConstColumnIterator<SparseMatrix2D>::operator bool() const {
    return isValid();
}

bool
ConstColumnIterator<SparseMatrix2D>::isValid() const {
    return column_ < numberOfNonZeroMatrixElements();
}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::numberOfNonZeroMatrixElements() const {
    size_type ncol = m_.nelements_[row_ + 1] - m_.nelements_[row_];
    return ncol;
}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::column() const {
    return column_mapped_;
}

ConstColumnIterator<SparseMatrix2D> &
ConstColumnIterator<SparseMatrix2D>::operator++() {
    // pre-increment
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column_mapped_, m_.cols() - 1);
#endif
    jumpToNextElement();
    return *this;
}

ConstColumnIterator<SparseMatrix2D>
ConstColumnIterator<SparseMatrix2D>::operator++(int) {
    // post-increment
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column_mapped_, m_.cols() - 1);
#endif
    auto tmp = *this;
    jumpToNextElement();
    return tmp;
}

double
ConstColumnIterator<SparseMatrix2D>::operator*() const {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column_mapped_, m_.cols() - 1);
#endif
    return m_(row_, column_mapped_);
}

void
ConstColumnIterator<SparseMatrix2D>::jumpToFirstElement() const {
    size_type offset = m_.nelements_[row_];
    column_mapped_ = m_.columns_[offset];
    column_ = 0;
}

void
ConstColumnIterator<SparseMatrix2D>::jumpToNextElement() const {
    size_type offset = m_.nelements_[row_];
    column_++;
    if (!isValid())
        return;
#ifdef _DEBUG
    size_type ncol = numberOfNonZeroMatrixElements();
    common_NS::reporting::checkUppderBound(column_, ncol - 1);
#endif
    column_mapped_ = m_.columns_[offset + column_];
}

} // namespace LinAlg_NS
