#include "pch.h"

#include "common/reporting.h"
#include "ConstColumnIterator.h"


namespace LinAlg_NS {


ConstColumnIterator<SparseMatrix2D>::ConstColumnIterator(SparseMatrix2D const & m, size_type row, size_type column)
    :
    m_(m), row_(row), column_(column) {

//        jumpToFirstElement();
}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::column() const {
    common_NS::reporting::checkUppderBound(column_, m_.cols());
    return column_;
}

double
ConstColumnIterator<SparseMatrix2D>::operator*() const {
    common_NS::reporting::checkUppderBound(row_, m_.rows());
    common_NS::reporting::checkUppderBound(column_, m_.cols());
    return m_(row_, column_);
}


#if 0

void t() {
    // Number of non-zero columns for this row
    size_type ncol = nelements_[row + 1] - nelements_[row];
    size_type offset = nelements_[row];

    double tmp = 0;

    // all non-zero columns
    for (size_type icol = 0; icol < ncol; ++icol) {
        size_type col = columns_[offset + icol];
        double a_ij = elements_[offset + icol];

        tmp += (a_ij * b(col));
    }

    x(row) = tmp;

}
#endif

} // namespace LinAlg_NS
