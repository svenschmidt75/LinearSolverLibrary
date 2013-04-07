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
    static ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>> operator*(double value, VECTOR_EXPR const & expr) {
        return ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>>(ScalarExpression(value), expr);
    }

    // Vector Expression * scalar
    template<typename VECTOR_EXPR>
    static ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>> operator*(VECTOR_EXPR const & expr, double value) {
        return ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>>(ScalarExpression(value), expr);
    }

    // Vector Expression + Vector Expression
    template<typename VECTOR_EXPR1, typename VECTOR_EXPR2>
    static VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, PLUS<double>> operator+(VECTOR_EXPR1 const & lhs, VECTOR_EXPR2 const & rhs) {
        // expression must be a vector to be compatible
        static_assert(typename expression_traits<VECTOR_EXPR1>::is_vector_expression::value == std::true_type::value, "lhs is not a vector");
        static_assert(typename expression_traits<VECTOR_EXPR2>::is_vector_expression::value == std::true_type::value, "rhs is not a vector");

        return VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, PLUS<double>>(lhs, rhs);
    }

    // scalar * Matrix, dense matrix
    static ScalarMatrixExpr<Matrix2D, MUL<double>> operator*(double value, Matrix2D const & m) {
        return ScalarMatrixExpr<Matrix2D, MUL<double>>(ScalarExpression(value), m);
    }

    // scalar * Matrix, sparse matrix
    static ScalarMatrixExpr<SparseMatrix2D, MUL<double>> operator*(double value, SparseMatrix2D const & m) {
        return ScalarMatrixExpr<SparseMatrix2D, MUL<double>>(ScalarExpression(value), m);
    }

    // Matrix * Vector Expression, dense and sparse
    template<typename MATRIX_EXPR, typename VECTOR_EXPR>
    static MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR> operator*(MATRIX_EXPR const & m, VECTOR_EXPR const & v) {
        return MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR>(m, v);
    }

} // namespace LinAlg_NS
