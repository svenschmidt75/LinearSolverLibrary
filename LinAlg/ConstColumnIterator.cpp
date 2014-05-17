#include "pch.h"

#include "ConstColumnIterator.h"
#include "common/reporting.h"


namespace LinAlg_NS {


ConstColumnIterator<SparseMatrix2D>::ConstColumnIterator(SparseMatrix2D const & m, size_type row)
    :
    m_{m},
    row_{row},
    column_{0},
    index_{0},
    column_indices_{m.getNonZeroColumnIndicesForRow(row)} {

#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
    jumpToFirstElement();
}

ConstColumnIterator<SparseMatrix2D> &
ConstColumnIterator<SparseMatrix2D>::operator=(ConstColumnIterator const & in) {
    const_cast<SparseMatrix2D &>(m_) = in.m_;
    column_                          = in.column_;
    row_                             = in.row_;
    index_                           = in.index_;
    column_indices_                  = in.column_indices_;
    return *this;
}

ConstColumnIterator<SparseMatrix2D>::operator bool() const {
    return isValid();
}

bool
ConstColumnIterator<SparseMatrix2D>::isValid() const {
    return column_ < m_.cols();
}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::nnz() const {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
    return column_indices_.size();
}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::column() const {
    return column_;
}

ConstColumnIterator<SparseMatrix2D> &
ConstColumnIterator<SparseMatrix2D>::operator++() {
    // pre-increment
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
    jumpToNextElement();
    return *this;
}

ConstColumnIterator<SparseMatrix2D>
ConstColumnIterator<SparseMatrix2D>::operator++(int) {
    // post-increment
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
    auto tmp = *this;
    jumpToNextElement();
    return tmp;
}

double
ConstColumnIterator<SparseMatrix2D>::operator*() const {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
    return m_(row_, column_);
}

void
ConstColumnIterator<SparseMatrix2D>::jumpToFirstElement() const {
    column_ = column_indices_.empty() ? m_.cols() : column_indices_[index_];
}

void
ConstColumnIterator<SparseMatrix2D>::jumpToNextElement() const {
    column_ = index_ == column_indices_.size() - 1 ? m_.cols() : column_indices_[++index_];
}

} // namespace LinAlg_NS
