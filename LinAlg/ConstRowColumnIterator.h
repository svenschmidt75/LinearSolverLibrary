/*
* Name  : ConstRowColumnIterator
* Path  :
* Use   : This iterator iterates over the rows of
*         sparse matrices. Its operator* returns
*         a ConstColumnIterator, which iterates over
*         the matrix elements.
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once

#include "DeclSpec.h"

#include "SparseMatrix2D.h"
#include "ConstRowColumnIteratorForwardDecl.h"
#include "ConstColumnIteratorForwardDecl.h"


namespace LinAlg_NS {


    template<>
    class LINALG_DECL_SYMBOLS ConstRowColumnIterator<SparseMatrix2D> {
    public:
        typedef SparseMatrix2D::size_type size_type;

    private:
        typedef ConstRowColumnIterator<SparseMatrix2D> iter;

    public:
        explicit ConstRowColumnIterator(SparseMatrix2D const & m);
        explicit ConstRowColumnIterator(SparseMatrix2D const & m, size_type row);
        ConstRowColumnIterator & operator=(ConstRowColumnIterator const & in);

        explicit operator bool() const;

        bool      isValid() const;
        size_type maxRows() const;
        size_type row() const;
        size_type nnz() const;

        iter & operator++();
        iter   operator++(int);

        ConstColumnIterator<SparseMatrix2D> operator*() const;

    private:
        SparseMatrix2D const & m_;
        mutable size_type      row_;
    };

} // namespace LinAlg_NS
