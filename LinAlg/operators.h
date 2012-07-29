/*
 * Name  : operators
 * Path  : 
 * Use   : Implements operator overloads for LinAlg objects.
 * Author: Sven Schmidt
 * Date  : 07/29/2011
 */
#pragma once

#include "DeclSpec.h"

#include "internal/VectorBinaryExpression.h"
#include "internal/ScalarExpression.h"
#include "internal/PrimitiveOperators.h"


namespace LinAlg_NS {

    using namespace internal;

    class Matrix2D;
    class Vector;
    class TransposeVector;
    class Expression;


    /* ScaleVectorExpression
     * 
     * Needs ScalarExpression, that returns double value.
     * VectorExpression, which returns vector element.
     * MatrixExpression which returns matrix element.
     */
    static VectorBinaryExpression<ScalarExpression, Vector, MUL<double>> operator*(double value, Vector const & v) {
        return VectorBinaryExpression<ScalarExpression, Vector, MUL<double>>(ScalarExpression(value, v.size()), v);
    }


//    VectorExpression operator*(MatrixExpression mexpr, Vector const & v);
//    VectorExpression operator*(MatrixExpression mexpr, VectorExpression vexpr);




//    Vector operator*(double value, Vector const & v);

    // dot product
//    double operator*(TransposeVector const & lhs, Vector const & rhs);

//    Vector operator*(Matrix2D const & m, Vector const & v);

//    Matrix2D operator*(TransposeVector const & v, Matrix2D const & m);

} // namespace LinAlg_NS
