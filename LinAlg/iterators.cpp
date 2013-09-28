#include "pch.h"

#include "iterators.h"
#include "ConstColumnIterator.h"

namespace LinAlg_NS {

using namespace iterator_internal;

ConstColumnIterator<SparseMatrix2D>
iterators::getConstIterator(SparseMatrix2D const & matrix) {
    return ConstColumnIterator<SparseMatrix2D>(matrix);
}

} // namespace LinAlg_NS
