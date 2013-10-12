#include "pch.h"

#include "common/reporting.h"
#include "ConstColumnIterator.h"


namespace LinAlg_NS {


ConstColumnIterator<SparseMatrix2D>::ConstColumnIterator(SparseMatrix2D const & m, size_type row)
    :
    m_(m), row_(row) {

    jumpToFirstElement();
}

bool
ConstColumnIterator<SparseMatrix2D>::isValid() const {
    common_NS::reporting::checkUppderBound(row_, m_.rows());
    size_type offset = m_.nelements_[row_];
    size_type ncol = m_.nelements_[row_ + 1] - offset;
    return column_ < ncol;
}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::column() const {
    common_NS::reporting::checkUppderBound(column_mapped_, m_.cols());
    return column_mapped_;
}

ConstColumnIterator<SparseMatrix2D> &
ConstColumnIterator<SparseMatrix2D>::operator++() {
    // pre-increment
    common_NS::reporting::checkUppderBound(row_, m_.rows());
    jumpToNextElement();
    common_NS::reporting::checkUppderBound(column_mapped_, m_.cols());
    return *this;
}

ConstColumnIterator<SparseMatrix2D>
ConstColumnIterator<SparseMatrix2D>::operator++(int) {
    // post-increment
    common_NS::reporting::checkUppderBound(row_, m_.rows());
    auto tmp = *this;
    jumpToNextElement();
    common_NS::reporting::checkUppderBound(column_mapped_, m_.cols());
    return tmp;
}

double
ConstColumnIterator<SparseMatrix2D>::operator*() const {
    common_NS::reporting::checkUppderBound(row_, m_.rows());
    common_NS::reporting::checkUppderBound(column_mapped_, m_.cols());
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
    common_NS::reporting::checkUppderBound(row_, m_.rows());
    size_type offset = m_.nelements_[row_];
    size_type ncol = m_.nelements_[row_ + 1] - offset;
    column_++;
    if (!isValid())
        return;
    common_NS::reporting::checkUppderBound(column_, ncol - 1);
    column_mapped_ = m_.columns_[offset + column_];
}

} // namespace LinAlg_NS
