/*
* Name  : ConstColumnIterator<SparseMatrix2D>
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/28/2013
*/
#pragma once

#include "DeclSpec.h"

#include "SparseMatrix2D.h"
#include "ConstColumnIteratorForwardDecl.h"


namespace LinAlg_NS {


    template<>
    class LINALG_DECL_SYMBOLS ConstColumnIterator<SparseMatrix2D> {
    public:
        using size_type = SparseMatrix2D::size_type;

    private:
        using iter = ConstColumnIterator<SparseMatrix2D>;

    public:
        ConstColumnIterator(SparseMatrix2D const & m, size_type row);
        ConstColumnIterator & operator=(ConstColumnIterator const & in);

        explicit operator bool() const;

        bool isValid() const;
        size_type numberOfNonZeroMatrixElements() const;
        size_type column() const;

        iter & operator++();
        iter   operator++(int);

        double operator*() const;

    private:
        void jumpToFirstElement() const;
        void jumpToNextElement() const;

    private:
        SparseMatrix2D const & m_;
        mutable size_type      column_;
        mutable size_type      column_mapped_;
        size_type              row_;
    };

} // namespace LinAlg_NS
