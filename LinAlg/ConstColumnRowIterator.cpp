#include "pch.h"

#include "ConstColumnRowIterator.h"
#include "ConstRowIterator.h"

#include "common/reporting.h"


namespace LinAlg_NS {

ConstColumnRowIterator<SparseMatrix2D>::ConstColumnRowIterator(SparseMatrix2D const & m)
    :
    ConstColumnRowIterator(m, 0) {}

ConstColumnRowIterator<SparseMatrix2D>::ConstColumnRowIterator(SparseMatrix2D const & m, size_type column)
    :
    m_(m),
    column_(column) {

    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
}

ConstColumnRowIterator<SparseMatrix2D> &
ConstColumnRowIterator<SparseMatrix2D>::operator=(ConstColumnRowIterator const & in) {
    const_cast<SparseMatrix2D &>(m_) = in.m_;
    column_                          = in.column_;
    return *this;
}

ConstColumnRowIterator<SparseMatrix2D>::operator bool() const {
    return isValid();
}

bool
ConstColumnRowIterator<SparseMatrix2D>::isValid() const {
    return column_ < maxColumns();
}

ConstColumnRowIterator<SparseMatrix2D>::size_type
ConstColumnRowIterator<SparseMatrix2D>::maxColumns() const {
    return m_.cols();
}

ConstColumnRowIterator<SparseMatrix2D>::size_type
ConstColumnRowIterator<SparseMatrix2D>::column() const {
    return column_;
}

ConstColumnRowIterator<SparseMatrix2D>::size_type
ConstColumnRowIterator<SparseMatrix2D>::numberOfNonZeroMatrixElements() const {
    return (*(*this)).numberOfNonZeroMatrixElements();
}

ConstColumnRowIterator<SparseMatrix2D>::iter &
ConstColumnRowIterator<SparseMatrix2D>::operator++() {
    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
    column_++;
    return *this;
}

ConstColumnRowIterator<SparseMatrix2D>::iter
ConstColumnRowIterator<SparseMatrix2D>::operator++(int) {
    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
    iter tmp(*this);
    column_++;
    return tmp;
}

ConstRowIterator<SparseMatrix2D>
ConstColumnRowIterator<SparseMatrix2D>::operator*() const {
    common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
    return ConstRowIterator<SparseMatrix2D>(m_, column_);
}

} // namespace LinAlg_NS
