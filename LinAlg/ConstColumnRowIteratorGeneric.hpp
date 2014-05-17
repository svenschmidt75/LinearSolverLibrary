/*
* Name  : ConstColumnRowIteratorGeneric
* Path  :
* Use   : This iterator iterates over the columns of
*         sparse matrices. Its operator* returns
*         a ConstRowIterator, which iterates over the matrix
*         elements.
* Author: Sven Schmidt
* Date  : 12/24/2013
*/
#pragma once

#include "DeclSpec.h"


namespace LinAlg_NS {


    template<typename MATRIX_EXPR>
    class ConstColumnRowIterator {
    public:
        typedef IMatrix2D::size_type size_type;

    private:
        typedef ConstColumnRowIterator<MATRIX_EXPR> iter;

    public:
        explicit ConstColumnRowIterator(MATRIX_EXPR const & m)
            :
            ConstColumnRowIterator(m, 0) {}

        explicit ConstColumnRowIterator(MATRIX_EXPR const & m, size_type column)
            :
            m_(m),
            column_(column) {

#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
        }

        ConstColumnRowIterator & operator=(ConstColumnRowIterator const & in) {
            const_cast<MATRIX_EXPR &>(m_) = in.m_;
            column_                       = in.column_;
            return *this;
        }

        explicit operator bool() const {
            return isValid();
        }

        bool isValid() const {
            return column_ < maxColumns();
        }

        size_type maxColumns() const {
            return m_.cols();
        }

        size_type column() const {
            return column_;
        }

        size_type nnz() const {
            return (*(*this)).nnz();
        }

        iter & operator++() {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
            column_++;
            return *this;
        }

        iter operator++(int) {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
            iter tmp(*this);
            column_++;
            return tmp;
        }

        ConstRowIterator<MATRIX_EXPR> operator*() const {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
            return ConstRowIterator<MATRIX_EXPR>(m_, column_);
        }

    private:
        MATRIX_EXPR const & m_;
        mutable size_type   column_;
    };

} // namespace LinAlg_NS
