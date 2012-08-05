/*
 * Name  : operators
 * Path  : 
 * Use   : Implements operator overloads for LinAlg objects.
 * Author: Sven Schmidt
 * Date  : 07/29/2012
 */
#pragma once

#include "DeclSpec.h"

#include "Matrix2D.h"

#include "internal/ScalarMatrixExpr.h"
#include "internal/VectorBinaryExpr.h"
#include "internal/ScalarVectorBinaryExpr.h"
#include "internal/ScalarVectorExpr.h"
#include "internal/ScalarExpression.h"
#include "internal/PrimitiveOperators.h"
#include "internal/expression_traits.h"


namespace LinAlg_NS {

    using namespace internal;

    class Matrix2D;
    class Vector;
    class TransposeVector;
    class Expression;


    // scalar * Vector
    static ScalarVectorExpr<Vector, MUL<double>> operator*(double value, Vector const & v) {
        return ScalarVectorExpr<Vector, MUL<double>>(ScalarExpression(value), v);
    }

    // scalar * Vector Expression
    template<typename VECTOR_EXPR>
    static ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>> operator*(double value, VECTOR_EXPR const & expr) {
        return ScalarVectorBinaryExpr<VECTOR_EXPR, MUL<double>>(ScalarExpression(value), expr);
    }

    // Vector + Vector
    static VectorBinaryExpr<Vector, Vector, PLUS<double>> operator+(Vector const & lhs, Vector const & rhs) {
        return VectorBinaryExpr<Vector, Vector, PLUS<double>>(lhs, rhs);
    }

    // Vector + Vector Expression
    template<typename VECTOR_EXPR>
    static VectorBinaryExpr<Vector, VECTOR_EXPR, PLUS<double>> operator+(Vector const & lhs, VECTOR_EXPR const & rhs) {
        // expression must be a vector to be compatible
        static_assert(typename expression_traits<VECTOR_EXPR>::is_vector_expression::value == std::true_type::value, "rhs is not a vector");

        return VectorBinaryExpr<Vector, VECTOR_EXPR, PLUS<double>>(lhs, rhs);
    }

    // Vector Expression + Vector
    template<typename VECTOR_EXPR>
    static VectorBinaryExpr<VECTOR_EXPR, Vector, PLUS<double>> operator+(VECTOR_EXPR const & lhs, Vector const & rhs) {
        // expression must be a vector to be compatible
        static_assert(typename expression_traits<VECTOR_EXPR>::is_vector_expression::value == std::true_type::value, "lhs is not a vector");

        return VectorBinaryExpr<VECTOR_EXPR, Vector, PLUS<double>>(lhs, rhs);
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




    // dot product






//    VectorExpression operator*(MatrixExpression mexpr, Vector const & v);
//    VectorExpression operator*(MatrixExpression mexpr, VectorExpression vexpr);




//    Vector operator*(double value, Vector const & v);

    // dot product
//    double operator*(TransposeVector const & lhs, Vector const & rhs);

//    Vector operator*(Matrix2D const & m, Vector const & v);

//    Matrix2D operator*(TransposeVector const & v, Matrix2D const & m);

} // namespace LinAlg_NS
