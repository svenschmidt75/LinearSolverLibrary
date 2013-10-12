/*
* Name  : iterators
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once


#include "DeclSpec.h"
#include "SparseMatrix2D.h"
#include "ConstColumnIterator.h"
#include "ConstRowIterator.h"


namespace LinAlg_NS {


class LINALG_DECL_SYMBOLS iterators {
public:
    static ConstRowIterator<SparseMatrix2D> getConstRowIterator(SparseMatrix2D const & matrix);
    static ConstRowIterator<SparseMatrix2D> getConstRowIterator(SparseMatrix2D const & matrix, IMatrix2D::size_type row);
};

} // namespace LinAlg_NS
