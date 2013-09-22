#include "pch.h"

#include "iterator_internal.h"


namespace LinAlg_NS {

namespace iterator_internal {

ConstColumnIterator<SparseMatrix2D>::ConstColumnIterator(SparseMatrix2D const & m) : m_(m) {}

ConstColumnIterator<SparseMatrix2D>::size_type
ConstColumnIterator<SparseMatrix2D>::size() const {
    return m_.cols();
}





} // namespace iterator_internal

} // namespace LinAlg_NS
