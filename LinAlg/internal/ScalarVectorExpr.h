/*
 * Name  : ScalarVectorExpr
 * Path  : 
 * Use   : Encapsulates a binary scalar-vector expression
 * Author: Sven Schmidt
 * Date  : 08/04/2012
 */
#pragma once

#include "ScalarExpression.h"

#include "internal/expression_traits.h"

#include <type_traits>

#include <boost/assert.hpp>


namespace LinAlg_NS {

    namespace internal {

        template<typename VECTOR_EXPR, typename BINOP>
        class ScalarVectorExpr {
        public:
            explicit ScalarVectorExpr(ScalarExpression const & lhs, VECTOR_EXPR const & rhs)
                :
                op1_(lhs),
                op2_(rhs) {
					// ensure rhs is a vector expression
					static_assert(typename expression_traits<VECTOR_EXPR>::is_vector_expression::value == std::true_type::value, "rhs is not a vector-like type");
				}

            ScalarVectorExpr(ScalarVectorExpr const & in)
                :
                op1_(in.op1_),
                op2_(in.op2_) {}

            Vector::size_type size() const {
                return op2_.size();
            }

            double operator()(Vector::size_type index) const {
                return BINOP()(op1_(index), op2_(index));
            }

        private:
            ScalarExpression op1_;
            VECTOR_EXPR      op2_;
            BINOP			 op_;
        };


    } // namespace internal

} // namespace LinAlg_NS
