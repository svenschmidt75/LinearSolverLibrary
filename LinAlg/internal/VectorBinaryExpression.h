/*
 * Name  : VectorBinaryExpression
 * Path  : 
 * Use   : Encapsulates a binary vector expression
 * Author: Sven Schmidt
 * Date  : 07/29/2011
 */
#pragma once

#include <boost/assert.hpp>


namespace LinAlg_NS {

    namespace internal {

        template<typename T1, typename T2, typename BINOP>
        class VectorBinaryExpression {
        public:
            VectorBinaryExpression(T1 op1, T2 op2)
                :
                op1_(op1),
                op2_(op2) {
                    BOOST_ASSERT_MSG(op1.size() == op2.size(), "VectorBinaryExpression: Vector size mismatch");
                }

            VectorBinaryExpression(VectorBinaryExpression const & in)
                :
                op1_(in.op1_),
                op2_(in.op2_),
                op_(in.op_) {}

            Vector::size_type size() const {
                return op1_.size();
            }

            double operator()(Vector::size_type index) const {
                return op_(op1_(index), op2_(index));
            }

        private:
            T1    op1_;
            T2    op2_;
            BINOP op_;
        };


    } // namespace internal

} // namespace LinAlg_NS
