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
        typedef SparseMatrix2D::size_type size_type;

    private:
        typedef ConstColumnIterator<SparseMatrix2D> iter;

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



    template<typename MATRIX_EXPR>
    class ConstColumnIterator {
    public:
        typedef IMatrix2D::size_type size_type;

    private:
        typedef ConstColumnIterator<MATRIX_EXPR> iter;

    public:
        ConstColumnIterator(MATRIX_EXPR const & m, size_type row)
            :
            m_(m), row_(row) {

            jumpToFirstElement();
        }

        ConstColumnIterator & operator=(ConstColumnIterator const & in) {
            const_cast<MATRIX_EXPR &>(m_) = in.m_;
            column_                       = in.column_;
            row_                          = in.row_;
            return *this;
        }

        explicit operator bool() const {
            return isValid();
        }

        bool isValid() const {
            return column_ < numberOfNonZeroMatrixElements();
        }

        size_type numberOfNonZeroMatrixElements() const {
            common_NS::reporting::checkUppderBound(row_, m_.rows());
            size_type ncol{0};
            for (size_type column = 0; column < m_.cols(); ++column)
            {
                if (m_(row_, column))
                    ++ncol;
            }
            return ncol;
        }

        size_type column() const {
            common_NS::reporting::checkUppderBound(column_, m_.cols());
            return column_;
        }

        iter & operator++() {
            // pre-increment
            common_NS::reporting::checkUppderBound(row_, m_.rows());
            jumpToNextElement();
            common_NS::reporting::checkUppderBound(column_, m_.cols());
            return *this;
        }

        iter operator++(int) {
            // post-increment
            common_NS::reporting::checkUppderBound(row_, m_.rows());
            auto tmp = *this;
            jumpToNextElement();
            common_NS::reporting::checkUppderBound(column_, m_.cols());
            return tmp;
        }

        double operator*() const {
            common_NS::reporting::checkUppderBound(row_, m_.rows());
            common_NS::reporting::checkUppderBound(column_, m_.cols());
            return m_(row_, column_);
        }

    private:
        void jumpToFirstElement() const {
            for (column_ = 0; column_ < m_.cols(); ++column_)
            {
                if (m_(row_, column_))
                    break;
            }
        }

        void jumpToNextElement() const {
            ++column_;
            for (; column_ < m_.cols(); ++column_)
            {
                if (m_(row_, column_))
                    break;
            }
        }

    private:
        MATRIX_EXPR const & m_;
        mutable size_type   column_;
        size_type           row_;
    };

} // namespace LinAlg_NS
