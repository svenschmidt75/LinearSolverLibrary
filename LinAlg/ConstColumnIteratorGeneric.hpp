/*
* Name  : ConstColumnIteratorGeneric
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 12/24/2013
*/
#pragma once

#include "DeclSpec.h"

#include "SparseMatrix2D.h"
#include "ConstColumnIteratorForwardDecl.h"


namespace LinAlg_NS {

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

#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
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
            return column_ < m_.cols();
        }

        size_type numberOfNonZeroMatrixElements() const {
            size_type ncol{0};
            for (size_type column = 0; column < m_.cols(); ++column)
            {
                if (m_(row_, column))
                    ++ncol;
            }
            return ncol;
        }

        size_type column() const {
            return column_;
        }

        iter & operator++() {
            // pre-increment
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
            jumpToNextElement();
            return *this;
        }

        iter operator++(int) {
            // post-increment
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
            auto tmp = *this;
            jumpToNextElement();
            return tmp;
        }

        double operator*() const {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
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