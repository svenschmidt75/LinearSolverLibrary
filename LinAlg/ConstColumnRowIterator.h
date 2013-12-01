/*
* Name  : ConstColumnRowIterator
* Path  :
* Use   : This iterator iterates over the columns of
*         sparse matrices. Its operator* returns
*         a ConstRowIterator, which iterates over the matrix
*         elements.
* Author: Sven Schmidt
* Date  : 11/29/2013
*/
#pragma once

#include "DeclSpec.h"

#include "SparseMatrix2D.h"
#include "ConstColumnRowIteratorBase.h"
#include "ConstRowIteratorBase.h"


namespace LinAlg_NS {

    template<>
    class LINALG_DECL_SYMBOLS ConstColumnRowIterator<SparseMatrix2D> {
    public:
        typedef SparseMatrix2D::size_type size_type;

    private:
        typedef ConstColumnRowIterator<SparseMatrix2D> iter;

    public:
        explicit ConstColumnRowIterator(SparseMatrix2D const & m);
        explicit ConstColumnRowIterator(SparseMatrix2D const & m, size_type column);
        ConstColumnRowIterator & operator=(ConstColumnRowIterator const & in);

        explicit operator bool() const;

        bool isValid() const;

        size_type maxColumns() const;
        size_type column() const;
        size_type numberOfNonZeroMatrixElements() const;

        iter & operator++();
        iter   operator++(int);

        ConstRowIterator<SparseMatrix2D> operator*() const;

    private:
        SparseMatrix2D const & m_;
        mutable size_type      column_;
    };

} // namespace LinAlg_NS
