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
#include "ConstRowColumnIteratorBase.h"
#include "ConstColumnIteratorBase.h"


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

        bool isValid() const;
        size_type maxRows() const;
        size_type row() const;
        size_type numberOfNonZeroMatrixElements() const;

        iter & operator++();
        iter   operator++(int);

        ConstColumnIterator<SparseMatrix2D> operator*() const;

    private:
        SparseMatrix2D const & m_;
        mutable size_type      row_;
    };



    template<typename MATRIX_EXPR>
    class ConstRowColumnIterator {
    public:
        typedef IMatrix2D::size_type size_type;

    private:
        typedef ConstRowColumnIterator<MATRIX_EXPR> iter;

    public:
        explicit ConstRowColumnIterator(MATRIX_EXPR const & m)
            :
            ConstRowColumnIterator(m, 0) {}

        explicit ConstRowColumnIterator(MATRIX_EXPR const & m, size_type row)
            :
            m_(m),
            row_(row) {}

        ConstRowColumnIterator & operator=(ConstRowColumnIterator const & in) {
            const_cast<MATRIX_EXPR &>(m_) = in.m_;
            row_                          = in.row_;
            return *this;
        }

        explicit operator bool() const {
            return isValid();
        }

        bool isValid() const {
            common_NS::reporting::checkUppderBound(row_, m_.rows());
            return row_ < maxRows();
        }

        size_type maxRows() const {
            return m_.rows();
        }

        size_type row() const {
            return row_;
        }

        size_type numberOfNonZeroMatrixElements() const {
            return (*(*this)).numberOfNonZeroMatrixElements();
        }

        iter & operator++() {
            row_++;
            return *this;
        }

        iter operator++(int) {
            iter tmp(*this);
            row_++;
            return tmp;
        }

        ConstColumnIterator<MATRIX_EXPR> operator*() const {
            return ConstColumnIterator<MATRIX_EXPR>(m_, row_);
        }

    private:
        MATRIX_EXPR const & m_;
        mutable size_type   row_;
    };


} // namespace LinAlg_NS
