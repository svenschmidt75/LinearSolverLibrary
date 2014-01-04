/*
* Name  : ConstColumnIteratorGeneric
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 12/24/2013
*/
#pragma once

#include "DeclSpec.h"


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
            m_(m),
            row_(row),
            column_{0},
            index_{0},
            column_indices_{m.getNonZeroColumnIndicesForRow(row)} {
#ifdef _DEBUG
            common_NS::reporting::checkUppderBound(row_, m_.rows() - 1);
#endif
            jumpToFirstElement();
        }

        ConstColumnIterator & operator=(ConstColumnIterator const & in) {
            const_cast<MATRIX_EXPR &>(m_) = in.m_;
            column_                       = in.column_;
            row_                          = in.row_;
            index_                        = in.index_;
            column_indices_               = in.column_indices_;
            return *this;
        }

        explicit operator bool() const {
            return isValid();
        }

        bool isValid() const {
            return column_ < m_.cols();
        }

        size_type numberOfNonZeroMatrixElements() const {
            return column_indices_.size();
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
            column_ = column_indices_.empty() ? m_.cols() : column_indices_[index_];
        }

        void jumpToNextElement() const {
            column_ = index_ == column_indices_.size() - 1 ? m_.cols() : column_indices_[++index_];
        }

    private:
        MATRIX_EXPR const &    m_;
        mutable size_type      column_;
        size_type              row_;
        mutable size_type      index_;
        std::vector<size_type> column_indices_;
    };

} // namespace LinAlg_NS
