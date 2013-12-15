/*
 * Name  : NonTrivialEntityOperators
 * Path  : 
 * Use   : Implements operator overloads for LinAlg objects.
 * Author: Sven Schmidt
 * Date  : 07/29/2012
 */
#pragma once

#ifndef ENTITY_OPERATORS
#error "Do not include TrivialEntityOperators.h directly, but EntityOperators.h instead"
#endif

#include "internal/MatrixVectorExpr.h"
#include "internal/MatrixMatrixMul.h"
#include "internal/ScalarMatrixExpr.h"
#include "internal/VectorBinaryExpr.h"
#include "internal/ScalarVectorExpr.h"
#include "internal/ScalarExpression.h"
#include "internal/PrimitiveOperators.h"
#include "internal/entity_traits.h"


namespace LinAlg_NS {

    using namespace internal;

    class Matrix2D;
    class SparseMatrix2D;
    class Vector;
    class TransposeVector;


    template<typename VECTOR_EXPR>
    static typename std::enable_if<
        entity_traits<VECTOR_EXPR>::is_vector_expression == true,
        typename ScalarVectorExpr<VECTOR_EXPR, MUL<double>>
    >::type
    operator*(double scalar, VECTOR_EXPR const & rhs) {
        return ScalarVectorExpr<VECTOR_EXPR, MUL<double>>(ScalarExpression(scalar), rhs);
    }

    template<typename VECTOR_EXPR>
    static typename std::enable_if<
        entity_traits<VECTOR_EXPR>::is_vector_expression == true,
        typename ScalarVectorExpr<VECTOR_EXPR, MUL<double>>
    >::type
    operator/(VECTOR_EXPR const & lhs, double scalar) {
        return ScalarVectorExpr<VECTOR_EXPR, MUL<double>>(ScalarExpression(1.0 / scalar), lhs);
    }

    /* We use SFINAE here to only make this method visible to the compiler for
     * vector-like types. Otherwise, whenever the compiler sees a + b, it causes
     * a compiler error.
     */
    template<typename VECTOR_EXPR1, typename VECTOR_EXPR2>
    static typename std::enable_if<
      entity_traits<VECTOR_EXPR1>::is_vector_expression == true &&
      entity_traits<VECTOR_EXPR2>::is_vector_expression == true,
      typename VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, ADD<double>>
    >::type
    operator+(VECTOR_EXPR1 const & lhs, VECTOR_EXPR2 const & rhs) {
        return VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, ADD<double>>(lhs, rhs);
    }

    template<typename VECTOR_EXPR1, typename VECTOR_EXPR2>
    static typename std::enable_if<
        entity_traits<VECTOR_EXPR1>::is_vector_expression == true &&
        entity_traits<VECTOR_EXPR2>::is_vector_expression == true,
        typename VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, SUB<double>>
    >::type
    operator-(VECTOR_EXPR1 const & lhs, VECTOR_EXPR2 const & rhs) {
        return VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, SUB<double>>(lhs, rhs);
    }

    template<typename MATRIX_EXPR>
    static typename std::enable_if<
        entity_traits<MATRIX_EXPR>::is_matrix_expression == true,
        typename ScalarMatrixExpr<MATRIX_EXPR, MUL<double>>
    >::type
    operator*(double scalar, MATRIX_EXPR const & rhs) {
        return ScalarMatrixExpr<MATRIX_EXPR, MUL<double>>(ScalarExpression(scalar), rhs);
    }

    template<typename MATRIX_EXPR, typename VECTOR_EXPR>
    static typename std::enable_if<
        entity_traits<MATRIX_EXPR>::is_matrix_expression == true &&
        entity_traits<VECTOR_EXPR>::is_vector_expression == true,
        typename MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR>
    >::type
    operator*(MATRIX_EXPR const & lhs, VECTOR_EXPR const & rhs) {
        return MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR>(lhs, rhs);
    }

    /* We use SFINAE here to only make this method visible to the compiler for
     * matrix-like types. Otherwise, whenever the compiler sees a + b, it causes
     * a compiler error.
     */
    template<typename NATRIX_EXPR_1, typename NATRIX_EXPR_2>
    static typename std::enable_if<
      entity_traits<NATRIX_EXPR_1>::is_matrix_expression == true &&
      entity_traits<NATRIX_EXPR_2>::is_matrix_expression == true,
      typename MatrixMatrixMul<NATRIX_EXPR_1, NATRIX_EXPR_1>
    >::type
    operator*(NATRIX_EXPR_1 const & lhs, NATRIX_EXPR_2 const & rhs) {
        return MatrixMatrixMul<NATRIX_EXPR_1, NATRIX_EXPR_2>(lhs, rhs);
    }

} // namespace LinAlg_NS
