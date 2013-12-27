/*
 * Name  : MatrixMatrixMul
 * Path  : 
 * Use   : Encapsulates a matrix-matrix expression (like matrix multiplication)
 * Author: Sven Schmidt
 * Date  : 12/15/2011
 */
#pragma once

#include "entity_traits.h"
#include "Vector.h"


namespace LinAlg_NS {

    namespace internal {

        template<typename MATRIX_EXPR_1, typename MATRIX_EXPR_2>
        class MatrixMatrixMul {
        public:
            using size_type = IMatrix2D::size_type;

        public:
            MatrixMatrixMul(MATRIX_EXPR_1 const & op1, MATRIX_EXPR_2 const & op2)
                :
                op1_(op1),
                op2_(op2) {
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
                double value = matrix_mul_traits<MATRIX_EXPR_1>::apply<MATRIX_EXPR_2>::op(op1_, op2_, row, column);
                return value;
            }

        private:
            MATRIX_EXPR_1 const & op1_;
            MATRIX_EXPR_2 const & op2_;
        };


    } // namespace internal

} // namespace LinAlg_NS

