/*
 * Name  : expression_traits
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/28/2011
 */
#pragma once

#include <type_traits>

#include "helper.h"


namespace LinAlg_NS {

    // forward-declarations
    class Vector;
    class Matrix2D;

namespace internal {

    // forward-declarations
    template<typename T1, typename T2, typename BINOP>
    class VectorBinaryExpr;

    template<typename VECTOR_EXPR, typename BINOP>
    class ScalarVectorExpr;

    template<typename VECTOR_EXPR, typename BINOP>
    class ScalarVectorBinaryExpr;

    template<typename MATRIX_EXPR, typename BINOP>
    class ScalarMatrixExpr;

    //////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct expression_traits {
        typedef std::false_type is_vector_expression;
    };

    template<>
    struct expression_traits<Vector> {
        typedef std::true_type is_vector_expression;
    };

    template<typename VECTOR_EXPR, typename BINOP>
    struct expression_traits<ScalarVectorExpr<VECTOR_EXPR, BINOP>> {
        typedef std::true_type is_vector_expression;
    };

    template<typename VECTOR_EXPR, typename BINOP>
    struct expression_traits<ScalarVectorBinaryExpr<VECTOR_EXPR, BINOP>> {
        typedef std::true_type is_vector_expression;
    };

	template<typename T1, typename T2, typename BINOP>
	struct expression_traits<VectorBinaryExpr<T1, T2, BINOP>> {
		typedef std::true_type is_vector_expression;
	};

    //////////////////////////////////////////////////////////////////////////////

    template<>
    struct expression_traits<Matrix2D> {
        typedef std::true_type is_matrix_expression;

        static double get_value(Matrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col) {
            return ::LinAlg_NS::helper::get_value(m, row, col);
        }
    };

    template<typename MATRIX_EXPR, typename BINOP>
    struct expression_traits<ScalarMatrixExpr<MATRIX_EXPR, BINOP>> {
        typedef std::true_type is_matrix_expression;
    };

} // namespace internal

} // namespace LinAlg_NS
