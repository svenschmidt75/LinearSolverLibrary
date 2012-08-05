/*
 * Name  : ScalarMatrixExpr
 * Path  : 
 * Use   : Encapsulates a binary scalar-matrix expression
 * Author: Sven Schmidt
 * Date  : 08/05/2011
 */
#pragma once

#include "ScalarExpression.h"

#include "expression_traits.h"

#include <boost/assert.hpp>


namespace LinAlg_NS {

    namespace internal {

        template<typename MATRIX_EXPR, typename BINOP>
        class ScalarMatrixExpr {
        public:
            ScalarMatrixExpr(ScalarExpression const & lhs, MATRIX_EXPR const & rhs)
                :
                op1_(lhs),
                op2_(rhs) {
					static_assert(typename expression_traits<MATRIX_EXPR>::is_matrix_expression::value == std::true_type::value, "rhs not a matrix-like type");
                }

            ScalarMatrixExpr(ScalarMatrixExpr const & in)
                :
                op1_(in.op1_),
                op2_(in.op2_) {}

            IMatrix2D::size_type rows() const {
                return op2_.rows();
            }

            IMatrix2D::size_type cols() const {
                return op2_.cols();
            }

            double operator()(IMatrix2D::size_type row, IMatrix2D::size_type col) const {
                // get matrix entry
                double value = expression_traits<MATRIX_EXPR>::get_value(op2_, row, col);
                return BINOP()(op1_(index), value);
            }

        private:
            ScalarExpression op1_;
            MATRIX_EXPR      op2_;
        };


    } // namespace internal

} // namespace LinAlg_NS
