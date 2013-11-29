#include "pch.h"

#include "common/reporting.h"
#include "ConstRowIterator.h"
#include "ConstColumnIterator.h"


namespace LinAlg_NS {


ConstRowIterator<SparseMatrix2D>::ConstRowIterator(SparseMatrix2D const & m)
    :
    m_(m), row_(0) {}

ConstRowIterator<SparseMatrix2D>::ConstRowIterator(SparseMatrix2D const & m, size_type row)
    :
    m_(m), row_(row) {}

bool 
ConstRowIterator<SparseMatrix2D>::isValid() const {
    common_NS::reporting::checkUppderBound(row_, m_.rows());
    return row_ < maxRows();
}

ConstRowIterator<SparseMatrix2D>::size_type
ConstRowIterator<SparseMatrix2D>::maxRows() const {
    return m_.rows();
}

ConstRowIterator<SparseMatrix2D>::size_type
ConstRowIterator<SparseMatrix2D>::row() const {
    return row_;
}

ConstRowIterator<SparseMatrix2D>::size_type
ConstRowIterator<SparseMatrix2D>::numberOfNonZeroMatrixElements() const {
    return (*(*this)).numberOfNonZeroMatrixElements();
}

ConstRowIterator<SparseMatrix2D> &
ConstRowIterator<SparseMatrix2D>::operator++() {
    row_++;
    return *this;
}

ConstRowIterator<SparseMatrix2D>
ConstRowIterator<SparseMatrix2D>::operator++(int) {
    iter tmp(*this);
    row_++;
    return tmp;
}

ConstColumnIterator<SparseMatrix2D>
ConstRowIterator<SparseMatrix2D>::operator*() const {
    return ConstColumnIterator<SparseMatrix2D>(m_, row_);
}


} // namespace LinAlg_NS
