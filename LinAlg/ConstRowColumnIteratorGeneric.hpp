/*
* Name  : ConstRowColumnIteratorGeneric
* Path  :
* Use   : This iterator iterates over the rows of
*         sparse matrices. Its operator* returns
*         a ConstColumnIterator, which iterates over
*         the matrix elements.
* Author: Sven Schmidt
* Date  : 12/24/2013
*/
#pragma once

#include "DeclSpec.h"

#include "ConstColumnIteratorForwardDecl.h"


namespace LinAlg_NS {


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
            row_(row) {

#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
        }

        ConstRowColumnIterator & operator=(ConstRowColumnIterator const & in) {
            const_cast<MATRIX_EXPR &>(m_) = in.m_;
            row_ = in.row_;
            return *this;
        }

        explicit operator bool() const {
            return isValid();
        }

        bool isValid() const {
            return row_ < maxRows();
        }

        size_type maxRows() const {
            return m_.rows();
        }

        size_type row() const {
            return row_;
        }

        size_type nnz() const {
            return (*(*this)).nnz();
        }

        iter & operator++() {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
            row_++;
            return *this;
        }

        iter operator++(int) {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
            iter tmp(*this);
            row_++;
            return tmp;
        }

        ConstColumnIterator<MATRIX_EXPR> operator*() const {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
            return ConstColumnIterator<MATRIX_EXPR>(m_, row_);
        }

    private:
        MATRIX_EXPR const & m_;
        mutable size_type   row_;
    };


} // namespace LinAlg_NS
