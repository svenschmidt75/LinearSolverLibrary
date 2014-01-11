/*
 * Name  : MatrixMatrixMul
 * Path  : 
 * Use   : Encapsulates a matrix-matrix expression (like matrix multiplication)
 * Author: Sven Schmidt
 * Date  : 12/15/2011
 */
#pragma once

#include "entity_traits.h"
#include "IMatrix2D.h"
#include "IndexMap.hpp"


namespace LinAlg_NS {

    namespace internal {

        template<typename MATRIX_EXPR_1, typename MATRIX_EXPR_2>
        class MatrixMatrixMul {
        public:
            using size_type = IMatrix2D::size_type;

        public:
            MatrixMatrixMul(MATRIX_EXPR_1 const & op1, MATRIX_EXPR_2 const & op2)
                :
                op1_{op1},
                op2_{op2},
                row_index_map_{std::bind(&MatrixMatrixMul::getNonZeroRowIndicesForColumn_internal, this, std::placeholders::_1), op2.cols()},
                column_index_map_{std::bind(&MatrixMatrixMul::getNonZeroColumnIndicesForRow_internal, this, std::placeholders::_1), op1.rows()} {
                    static_assert(typename entity_traits<MATRIX_EXPR_1>::is_matrix_expression == true, "op1 not a matrix-like type");
                    static_assert(typename entity_traits<MATRIX_EXPR_2>::is_matrix_expression == true, "op2 not a vector-like type");
                    common_NS::reporting::checkConditional(op1.cols() == op2.rows(), "MatrixMatrixMul: Matrices incompatible");
                }

            MatrixMatrixMul(MatrixMatrixMul const & in)
                :
                op1_(in.op1_),
                op2_(in.op2_) {}

            MatrixMatrixMul & operator=(MatrixMatrixMul const & in) {
                const_cast<MATRIX_EXPR_1 &>(op1_) = in.op1_;
                const_cast<MATRIX_EXPR_2 &>(op2_) = in.op2_;
                return *this;
            }

            size_type rows() const {
                return op1_.rows();
            }

            size_type cols() const {
                return op2_.cols();
            }

            double operator()(size_type row, size_type column) const {
                auto const & column_indices = op1_.getNonZeroColumnIndicesForRow(row);
                auto const & row_indices = op2_.getNonZeroRowIndicesForColumn(column);
                std::vector<IMatrix2D::size_type> non_zero_indices(op1_.cols());
                auto end_it = std::set_intersection(std::cbegin(column_indices), std::cend(column_indices), std::cbegin(row_indices), std::cend(row_indices), std::begin(non_zero_indices));
                if (non_zero_indices.empty())
                    return 0.0;
                double value = matrix_mul_traits<MATRIX_EXPR_1>::apply<MATRIX_EXPR_2>::op(op1_, op2_, row, column);
                return value;
            }

            std::vector<size_type> getNonZeroColumnIndicesForRow(size_type row) const {
                return column_index_map_.getIndices(row);
            }

            std::vector<size_type> getNonZeroRowIndicesForColumn(size_type column) const {
                return row_index_map_.getIndices(column);
            }

        private:
            std::vector<size_type> getNonZeroColumnIndicesForRow_internal(size_type row) const {
                auto const & op1_column_indices = op1_.getNonZeroColumnIndicesForRow(row);
                std::vector<IMatrix2D::size_type> non_zero_indices;
                non_zero_indices.reserve(cols());
                for (size_type column = 0; column < cols(); ++column) {
                    auto const & op2_row_indices = op2_.getNonZeroRowIndicesForColumn(column);
                    std::vector<IMatrix2D::size_type> tmp;
                    tmp.reserve(op2_row_indices.size());
                    std::set_intersection(std::cbegin(op1_column_indices), std::cend(op1_column_indices), std::cbegin(op2_row_indices), std::cend(op2_row_indices), std::back_insert_iterator<std::vector<IMatrix2D::size_type>>{tmp});
                    if (tmp.empty() == false)
                        non_zero_indices.push_back(column);
                }
                return non_zero_indices;
            }

            std::vector<size_type> getNonZeroRowIndicesForColumn_internal(size_type column) const {
                auto const & op2_row_indices = op2_.getNonZeroRowIndicesForColumn(column);
                std::vector<IMatrix2D::size_type> non_zero_indices;
                non_zero_indices.reserve(rows());
                for (size_type row = 0; row < rows(); ++row) {
                    auto const & op1_column_indices = op1_.getNonZeroColumnIndicesForRow(row);
                    std::vector<IMatrix2D::size_type> tmp;
                    tmp.reserve(op1_column_indices.size());
                    std::set_intersection(std::cbegin(op1_column_indices), std::cend(op1_column_indices), std::cbegin(op2_row_indices), std::cend(op2_row_indices), std::back_insert_iterator<std::vector<IMatrix2D::size_type>>{tmp});
                    if (tmp.empty() == false)
                        non_zero_indices.push_back(row);
                }
                return non_zero_indices;
            }

        private:
            IndexMap              row_index_map_;
            IndexMap              column_index_map_;
            MATRIX_EXPR_1 const & op1_;
            MATRIX_EXPR_2 const & op2_;
        };

    } // namespace internal

} // namespace LinAlg_NS

