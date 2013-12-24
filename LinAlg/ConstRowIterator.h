/*
* Name  : ConstRowIterator<SparseMatrix2D> and
*         template<typename MATRIX_EXPR>
*         ConstRowIterator<MATRIX_EXPR>
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
    };



    template<typename MATRIX_EXPR>
    class ConstRowIterator {
    public:
        typedef IMatrix2D::size_type size_type;

    private:
        typedef ConstRowIterator<MATRIX_EXPR> iter;

    public:
        ConstRowIterator(MATRIX_EXPR const & m, size_type column)
            :
            m_{m},
            column_{column},
            row_{0} {

            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
            jumpToFirstElement();
        }

        ConstRowIterator & operator=(ConstRowIterator const & in) {
            const_cast<MATRIX_EXPR &>(m_) = in.m_;
            column_                       = in.column_;
            row_                          = in.row_;
            return *this;
        }

        explicit operator bool() const {
            return isValid();
        }

        bool isValid() const {
            return row_ < m_.rows();
        }

        size_type numberOfNonZeroMatrixElements() const {
            auto nelements = 0;
            for (auto row = 0; row < m_.rows(); ++row) {
                // find the 1st non-zero element in column 'column_'
                if (m_(row, column_))
                    ++nelements;
            }
            return nelements;
        }

        size_type row() const {
            common_NS::reporting::checkConditional(isValid(), "Iterator is in an invalid state");
            return row_;
        }

        iter & operator++() {
            // pre-increment
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
            jumpToNextElement();
            common_NS::reporting::checkUppderBound(row_, m_.rows());
            return *this;
        }

        iter operator++(int) {
            // post-increment
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
            auto tmp = *this;
            jumpToNextElement();
            common_NS::reporting::checkUppderBound(row_, m_.rows());
            return tmp;
        }

        double operator*() const {
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
            return m_(row_, column_);
        }

    private:
        void jumpToFirstElement() const {
            for (row_ = 0; row_ < m_.rows(); ++row_) {
                // find the 1st non-zero element in column 'column_'
                if (m_(row_, column_))
                    break;
            }
        }

        void jumpToNextElement() const {
            ++row_;
            jumpToFirstElement();
        }

    private:
        MATRIX_EXPR const & m_;
        size_type           column_;
        mutable size_type   row_;
    };

} // namespace LinAlg_NS
