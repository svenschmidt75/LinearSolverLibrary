/*
 * Name  : MatrixVectorExpr
 * Path  : 
 * Use   : Encapsulates a binary scalar-vector expression
 * Author: Sven Schmidt
 * Date  : 08/04/2011
 */
#pragma once

#include "expression_traits.h"

#include <boost/assert.hpp>


namespace LinAlg_NS {

    namespace internal {

        template<typename MATRIX_EXPR, typename VECTOR_EXPR>
        class MatrixVectorExpr {
        public:
            MatrixVectorExpr(MATRIX_EXPR const & m, VECTOR_EXPR const & v)
                :
                op1_(m),
                op2_(v) {
                    static_assert(typename expression_traits<MATRIX_EXPR>::is_matrix_expression::value == std::true_type::value, "lhs not a matrix-like type");
					static_assert(typename expression_traits<VECTOR_EXPR>::is_vector_expression::value == std::true_type::value, "rhs not a vector-like type");
                }

            MatrixVectorExpr(MatrixVectorExpr const & in)
                :
                op1_(in.op1_),
                op2_(in.op2_) {}

            Vector::size_type size() const {
                return op1_.rows();
            }

            double operator()(Vector::size_type index) const {
                // return row
                double value = matrix_operation_traits<MATRIX_EXPR>::apply<VECTOR_EXPR>::op(op1_, op2_, index);
                return value;
            }

        private:
            MATRIX_EXPR op1_;
            VECTOR_EXPR op2_;
        };


    } // namespace internal

} // namespace LinAlg_NS
