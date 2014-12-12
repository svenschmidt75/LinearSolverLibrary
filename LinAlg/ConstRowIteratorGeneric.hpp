/*
* Name  : ConstRowIteratorGeneric
* Path  :
* Use   : This iterator moves down a fixed column
*         in a sparse matrix, skipping non-zero
*         entries.
*
* Author: Sven Schmidt
* Date  : 12/24/2013
*/
#pragma once

#include "DeclSpec.h"


namespace LinAlg_NS {


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
            row_{0},
            index_{0},
            row_indices_(m.getNonZeroRowIndicesForColumn(column)) {

#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(column_, m_.cols() - 1);
#endif
            jumpToFirstElement();
        }

        ConstRowIterator & operator=(ConstRowIterator const & in) {
            const_cast<MATRIX_EXPR &>(m_) = in.m_;
            column_                       = in.column_;
            row_                          = in.row_;
            index_                        = in.index_;
            row_indices_                  = in.row_indices_;
            return *this;
        }

        explicit operator bool() const {
            return isValid();
        }

        bool isValid() const {
            return row_ < m_.rows();
        }

        size_type nnz() const {
            return row_indices_.size();
        }

        size_type row() const {
            return row_;
        }

        iter & operator++() {
            // pre-increment
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
            jumpToNextElement();
            return *this;
        }

        iter operator++(int) {
            // post-increment
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
            auto tmp = *this;
            jumpToNextElement();
            return tmp;
        }

        double operator*() const {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
            return m_(row_, column_);
        }

    private:
        void jumpToFirstElement() const {
            row_ = row_indices_.empty() ? m_.rows() : row_indices_[index_];
        }

        void jumpToNextElement() const {
            row_ = index_ == row_indices_.size() - 1 ? m_.rows() : row_indices_[++index_];
        }

    private:
        MATRIX_EXPR const &    m_;
        size_type              column_;
        mutable size_type      row_;
        mutable size_type      index_;
        std::vector<size_type> row_indices_;
    };

} // namespace LinAlg_NS
