/*
* Name  : ConstRowIterator<SparseMatrix2D>
* Path  :
* Use   : This iterator moves down a fixed column
*         in a sparse matrix, skipping non-zero
*         entries.
* 
* Author: Sven Schmidt
* Date  : 11/29/2013
*/
#pragma once

#include "DeclSpec.h"

#include "SparseMatrix2D.h"
#include "ConstRowIteratorForwardDecl.h"


#pragma warning(disable:4251)


namespace LinAlg_NS {


    template<>
    class LINALG_DECL_SYMBOLS ConstRowIterator<SparseMatrix2D> {
    public:
        typedef SparseMatrix2D::size_type size_type;

    private:
        typedef ConstRowIterator<SparseMatrix2D> iter;

    public:
        ConstRowIterator(SparseMatrix2D const & m, size_type column);
        ConstRowIterator & operator=(ConstRowIterator const & in);

        explicit operator bool() const;

        bool isValid() const;
        size_type numberOfNonZeroMatrixElements() const;
        size_type row() const;

        iter & operator++();
        iter   operator++(int);

        double operator*() const;

    private:
        void jumpToFirstElement() const;
        void jumpToNextElement() const;

    private:
        SparseMatrix2D const & m_;
        size_type              column_;
        mutable size_type      row_;
        std::set<size_type>    row_indices_;
    };

} // namespace LinAlg_NS

#pragma warning(default:4251)
