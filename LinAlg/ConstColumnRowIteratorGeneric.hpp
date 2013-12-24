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

#include "SparseMatrix2D.h"
#include "ConstColumnRowIteratorForwardDecl.h"
#include "ConstRowIteratorForwardDecl.h"


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

            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
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

        size_type numberOfNonZeroMatrixElements() const {
            return (*(*this)).numberOfNonZeroMatrixElements();
        }

        iter & operator++() {
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
            column_++;
            return *this;
        }

        iter operator++(int) {
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
            iter tmp(*this);
            column_++;
            return tmp;
        }

        ConstRowIterator<MATRIX_EXPR> operator*() const {
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
            return ConstRowIterator<MATRIX_EXPR>(m_, column_);
        }

    private:
        MATRIX_EXPR const & m_;
        mutable size_type   column_;
    };

} // namespace LinAlg_NS
