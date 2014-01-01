#include "pch.h"

#include "ConstRowIterator.h"
#include "common/reporting.h"


using namespace LinAlg_NS;


ConstRowIterator<SparseMatrix2D>::ConstRowIterator(SparseMatrix2D const & m, size_type column)
    :
    m_{m},
    column_{column},
    row_{0} {

#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif

    jumpToFirstElement();
}

ConstRowIterator<SparseMatrix2D> &
ConstRowIterator<SparseMatrix2D>::operator=(ConstRowIterator const & in) {
    const_cast<SparseMatrix2D &>(m_) = in.m_;
    column_                          = in.column_;
    row_                             = in.row_;
    return *this;
}

ConstRowIterator<SparseMatrix2D>::operator bool() const {
    return isValid();
}

bool
ConstRowIterator<SparseMatrix2D>::isValid() const {
    return row_ < m_.rows();
}

ConstRowIterator<SparseMatrix2D>::size_type
ConstRowIterator<SparseMatrix2D>::numberOfNonZeroMatrixElements() const {
    auto nelements = 0;
    for (auto row = 0; row < m_.rows(); ++row) {
        size_type ncolumns = m_.columns_offset_[row + 1] - m_.columns_offset_[row];
        size_type offset = m_.columns_offset_[row];
        size_type column;
        for (auto ncol = 0; ncol < ncolumns; ++ncol) {
            column = m_.columns_[offset + ncol];
            if (column == column_) {
                nelements++;
                break;
            }
            if (column > column_)
                break;
        }
    }
    return nelements;
}

ConstRowIterator<SparseMatrix2D>::size_type
ConstRowIterator<SparseMatrix2D>::row() const {
    return row_;
}

ConstRowIterator<SparseMatrix2D>::iter &
ConstRowIterator<SparseMatrix2D>::operator++() {
    // pre-increment
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
    jumpToNextElement();
    return *this;
}

ConstRowIterator<SparseMatrix2D>::iter
ConstRowIterator<SparseMatrix2D>::operator++(int) {
    // post-increment
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
    auto tmp = *this;
    jumpToNextElement();
    return tmp;
}

double
ConstRowIterator<SparseMatrix2D>::operator*() const {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
    return m_(row_, column_);
}

void
ConstRowIterator<SparseMatrix2D>::jumpToFirstElement() const {
    row_indices_ = m_.getNonZeroRowIndicesForColumn(column_);
    row_ = *std::cbegin(row_indices_);
}

void
ConstRowIterator<SparseMatrix2D>::jumpToNextElement() const {
    auto it = row_indices_.find(row_);
#ifdef _DEBUG
    common_NS::reporting::checkConditional(it != std::cend(row_indices_));
#endif
    ++it;
    row_ = it == std::cend(row_indices_) ? m_.rows() : *it;
}
