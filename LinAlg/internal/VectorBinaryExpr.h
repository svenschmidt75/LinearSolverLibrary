/*
 * Name  : VectorBinaryExpr
 * Path  : 
 * Use   : Encapsulates a binary vector expression
 * Author: Sven Schmidt
 * Date  : 07/29/2011
 */
#pragma once

#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>


namespace LinAlg_NS {

    namespace internal {

        template<typename T1, typename T2, typename BINOP>
        class VectorBinaryExpr : boost::noncopyable {
        public:
            VectorBinaryExpr(T1 const & op1, T2 const & op2)
                :
                op1_(op1),
                op2_(op2) {
                    BOOST_ASSERT_MSG(op1.size() == op2.size(), "VectorBinaryExpression: Vector size mismatch");
					static_assert(typename expression_traits<T1>::is_vector_expression::value == std::true_type::value, "op1 not a vector-like type");
					static_assert(typename expression_traits<T2>::is_vector_expression::value == std::true_type::value, "op2 not a vector-like type");
                }

            VectorBinaryExpr(VectorBinaryExpr const & in)
                :
                op1_(in.op1_),
                op2_(in.op2_) {}

            Vector::size_type size() const {
                return op1_.size();
            }

            double operator()(Vector::size_type index) const {
                return BINOP()(op1_(index), op2_(index));
            }

        private:
            T1 const & op1_;
            T2 const & op2_;
        };


    } // namespace internal

} // namespace LinAlg_NS
