/*
 * Name  : operators
 * Path  : 
 * Use   : Implements operator overloads for LinAlg objects.
 * Author: Sven Schmidt
 * Date  : 07/29/2012
 */
#pragma once

#include "DeclSpec.h"

#include "internal/VectorBinaryExpression.h"
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
//     template<typename EXPR>
//     static VectorBinaryExpression<ScalarExpression, EXPR, MUL<double>> operator*(double value, EXPR expr) {
//         return VectorBinaryExpression<ScalarExpression, EXPR, MUL<double>>(ScalarExpression(value, expr.size()), expr);
//     }

    // Vector + Vector
    static VectorBinaryExpression<Vector, Vector, PLUS<double>> operator+(Vector const & lhs, Vector const & rhs) {
        return VectorBinaryExpression<Vector, Vector, PLUS<double>>(lhs, rhs);
    }

    // Vector + Expression
    template<typename EXPR>
    static VectorBinaryExpression<Vector, EXPR, PLUS<double>> operator+(Vector const & lhs, EXPR rhs) {
        // expression must be a vector to be compatible
        static_assert(typename expression_traits<EXPR>::is_vector_expression::value == std::true_type::value, "rhs is not a vector");

        return VectorBinaryExpression<Vector, EXPR, PLUS<double>>(lhs, rhs);
    }

    // Expression + Vector
    template<typename EXPR>
    static VectorBinaryExpression<EXPR, Vector, PLUS<double>> operator+(EXPR lhs, Vector const & rhs) {
        // expression must be a vector to be compatible
        static_assert(typename expression_traits<EXPR>::is_vector_expression::value == std::true_type::value, "lhs is not a vector");

        return VectorBinaryExpression<EXPR, Vector, PLUS<double>>(lhs, rhs);
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
