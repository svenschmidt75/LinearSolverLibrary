/*
 * Name  : ScalarVectorExpr
 * Path  : 
 * Use   : Encapsulates a binary scalar-vector expression
 * Author: Sven Schmidt
 * Date  : 08/04/2011
 */
#pragma once

#include "ScalarExpression.h"

#include "entity_traits.h"


namespace LinAlg_NS {

    namespace internal {

        template<typename VECTOR_EXPR, typename BINOP>
        class ScalarVectorExpr : boost::noncopyable {
        public:
            ScalarVectorExpr(ScalarExpression const & lhs, VECTOR_EXPR const & rhs)
                :
                op1_(lhs),
                op2_(rhs) {
                    static_assert(typename entity_traits<VECTOR_EXPR>::is_vector_expression == true, "rhs not a vector-like type");
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
            ScalarExpression    op1_;
            VECTOR_EXPR const & op2_;
        };


    } // namespace internal

} // namespace LinAlg_NS
