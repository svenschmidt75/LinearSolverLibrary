/*
 * Name  : expression_traits
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/28/2011
 */
#pragma once

#include <type_traits>


namespace LinAlg_NS {

namespace internal {

    // forward-declarations
    template<typename T1, typename T2, typename BINOP>
    class VectorBinaryExpression;


    template<typename T>
    struct expression_traits {
        typedef std::false_type is_vector_expression;
    };

    template<>
    struct expression_traits<Vector> {
        typedef std::true_type is_vector_expression;
    };

	template<typename T1, typename T2, typename BINOP>
	struct expression_traits<VectorBinaryExpression<T1, T2, BINOP>> {
		typedef std::true_type is_vector_expression;
	};


} // namespace internal

} // namespace LinAlg_NS
