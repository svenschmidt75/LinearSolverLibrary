#include "pch.h"

#include "iterators.h"
#include "ConstColumnIterator.h"

namespace LinAlg_NS {


ConstRowIterator<SparseMatrix2D>
iterators::getConstRowIterator(SparseMatrix2D const & matrix) {
    return ConstRowIterator<SparseMatrix2D>(matrix);
}


} // namespace LinAlg_NS
