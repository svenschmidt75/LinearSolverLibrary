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
#include "ConstRowColumnIteratorForwardDecl.h"
#include "ConstColumnRowIteratorForwardDecl.h"


namespace LinAlg_NS {

    template<>
    class ConstRowColumnIterator<SparseMatrix2D>;

    template<>
    class ConstColumnRowIterator<SparseMatrix2D>;


class LINALG_DECL_SYMBOLS MatrixIterators {
public:
    static ConstRowColumnIterator<SparseMatrix2D> getConstRowColumnIterator(SparseMatrix2D const & matrix);
    static ConstRowColumnIterator<SparseMatrix2D> getConstRowColumnIterator(SparseMatrix2D const & matrix, IMatrix2D::size_type row);

    template<typename MATRIX_EXPR>
    static ConstRowColumnIterator<MATRIX_EXPR>
    getConstRowColumnIterator(MATRIX_EXPR const & matrix_expr) {
        return ConstRowColumnIterator<MATRIX_EXPR>(matrix_expr);
    }

    template<typename MATRIX_EXPR>
    static ConstRowColumnIterator<MATRIX_EXPR>
    getConstRowColumnIterator(MATRIX_EXPR const & matrix_expr, IMatrix2D::size_type row) {
        return ConstRowColumnIterator<MATRIX_EXPR>(matrix_expr, row);
    }

    static ConstColumnRowIterator<SparseMatrix2D> getConstColumnRowIterator(SparseMatrix2D const & matrix);
    static ConstColumnRowIterator<SparseMatrix2D> getConstColumnRowIterator(SparseMatrix2D const & matrix, IMatrix2D::size_type column);

    template<typename MATRIX_EXPR>
    static ConstColumnRowIterator<MATRIX_EXPR>
    getConstColumnRowIterator(MATRIX_EXPR const & matrix_expr) {
        return ConstColumnRowIterator<MATRIX_EXPR>(matrix_expr);
    }

    template<typename MATRIX_EXPR>
    static ConstColumnRowIterator<MATRIX_EXPR>
    getConstColumnRowIterator(MATRIX_EXPR const & matrix_expr, IMatrix2D::size_type column) {
        return ConstColumnRowIterator<MATRIX_EXPR>(matrix_expr, column);
    }
};

} // namespace LinAlg_NS
