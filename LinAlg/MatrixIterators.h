/*
* Name  : MatrixIterators
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once

#include "DeclSpec.h"

#include "SparseMatrix2D.h"
#include "ConstRowColumnIteratorBase.h"
#include "ConstColumnRowIteratorBase.h"


namespace LinAlg_NS {

    template<>
    class ConstRowColumnIterator<SparseMatrix2D>;

    template<>
    class ConstColumnRowIterator<SparseMatrix2D>;


class LINALG_DECL_SYMBOLS MatrixIterators {
public:
    static ConstRowColumnIterator<SparseMatrix2D> getConstRowColumnIterator(SparseMatrix2D const & matrix);
    static ConstRowColumnIterator<SparseMatrix2D> getConstRowColumnIterator(SparseMatrix2D const & matrix, IMatrix2D::size_type row);
    static ConstColumnRowIterator<SparseMatrix2D> getConstColumnRowIterator(SparseMatrix2D const & matrix);
    static ConstColumnRowIterator<SparseMatrix2D> getConstColumnRowIterator(SparseMatrix2D const & matrix, IMatrix2D::size_type column);
};

} // namespace LinAlg_NS
