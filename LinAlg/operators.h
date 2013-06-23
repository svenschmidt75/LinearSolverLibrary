/*
 * Name  : operators
 * Path  : 
 * Use   : Implements operator overloads for LinAlg objects.
 * Author: Sven Schmidt
 * Date  : 07/29/2012
 */
#pragma once

#include "DeclSpec.h"

#include "internal/MatrixVectorExpr.h"
#include "internal/ScalarMatrixExpr.h"
#include "internal/VectorBinaryExpr.h"
#include "internal/ScalarVectorBinaryExpr.h"
#include "internal/ScalarExpression.h"
#include "internal/PrimitiveOperators.h"
#include "internal/expression_traits.h"


namespace LinAlg_NS {

    using namespace internal;

    class Matrix2D;
    class SparseMatrix2D;
    class Vector;
    class TransposeVector;

    // scalar * Vector Expression
    template<typename VECTOR_EXPR>
    static typename ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>> operator*(double value, VECTOR_EXPR const & expr) {
        return ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>>(internal::ScalarExpression(value), expr);
    }

    // Vector Expression * scalar
    template<typename VECTOR_EXPR>
    static typename ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>> operator*(VECTOR_EXPR const & expr, double value) {
        return ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>>(internal::ScalarExpression(value), expr);
    }

    static VectorBinaryExpr<Vector, Vector, PLUS<double>>
    inline operator+(Vector const & lhs, Vector const & rhs) {
        return VectorBinaryExpr<Vector, Vector, PLUS<double>>(lhs, rhs);
    }

    template<typename VECTOR_EXPR1, typename VECTOR_EXPR2, typename BINOP>
    static VectorBinaryExpr<VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, BINOP>, Vector, internal::PLUS<double>>
    operator+(VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, BINOP> const & lhs, Vector const & rhs) {
        return VectorBinaryExpr<VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, BINOP>, Vector, PLUS<double>>(lhs, rhs);
    }

    template<typename VECTOR_EXPR1, typename VECTOR_EXPR2, typename BINOP1, typename VECTOR_EXPR3, typename VECTOR_EXPR4, typename BINOP2>
    static VectorBinaryExpr<VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, BINOP1>, VectorBinaryExpr<VECTOR_EXPR3, VECTOR_EXPR4, BINOP2>, PLUS<double>>
    operator+(VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, BINOP1> const & lhs, VectorBinaryExpr<VECTOR_EXPR3, VECTOR_EXPR4, BINOP2> const & rhs) {
        return VectorBinaryExpr<VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, BINOP1>, VectorBinaryExpr<VECTOR_EXPR3, VECTOR_EXPR4, BINOP2>, PLUS<double>>(lhs, rhs);
    }

    template<typename VECTOR_EXPR, typename BINOP>
    static VectorBinaryExpr<Vector, ScalarVectorBinaryExpr<VECTOR_EXPR, BINOP>, PLUS<double>>
    operator+(Vector const & lhs, ScalarVectorBinaryExpr<VECTOR_EXPR, BINOP> const & rhs) {
        return VectorBinaryExpr<Vector, ScalarVectorBinaryExpr<VECTOR_EXPR, BINOP>, PLUS<double>>(lhs, rhs);
    }

    /* If VS would better work with templates, we could say
    * 
    template<typename VECTOR_EXPR1, typename VECTOR_EXPR2>
    static typename std::enable_if<
    internal::expression_traits<typename std::remove_const<typename std::remove_reference<VECTOR_EXPR1>::type>::type>::is_vector_expression::value == std::true_type::value &&
    internal::expression_traits<typename std::remove_const<typename std::remove_reference<VECTOR_EXPR2>::type>::type>::is_vector_expression::value == std::true_type::value,
    typename internal::VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, internal::PLUS<double>>>::type
    operator+(VECTOR_EXPR1 const & lhs, VECTOR_EXPR2 const & rhs)
    {
    // expression must be a vector to be compatible
    static_assert(internal::expression_traits<VECTOR_EXPR1>::is_vector_expression::value == std::true_type::value, "lhs is not a vector");
    static_assert(internal::expression_traits<VECTOR_EXPR2>::is_vector_expression::value == std::true_type::value, "rhs is not a vector");

    return internal::VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, internal::PLUS<double>>(lhs, rhs);
    }
     */
    
    // scalar * Matrix, dense matrix
    static inline ScalarMatrixExpr<Matrix2D, MUL<double>> operator*(double value, Matrix2D const & m) {
        return ScalarMatrixExpr<Matrix2D, MUL<double>>(ScalarExpression(value), m);
    }

    // scalar * Matrix, sparse matrix
    static inline ScalarMatrixExpr<SparseMatrix2D, MUL<double>> operator*(double value, SparseMatrix2D const & m) {
        return ScalarMatrixExpr<SparseMatrix2D, MUL<double>>(ScalarExpression(value), m);
    }

    // Matrix * Vector Expression, dense and sparse
    template<typename MATRIX_EXPR, typename VECTOR_EXPR>
    static typename MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR> operator*(MATRIX_EXPR const & m, VECTOR_EXPR const & v) {
        return MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR>(m, v);
    }

} // namespace LinAlg_NS
