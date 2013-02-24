/*
 * Name  : entities
 * Path  : 
 * Use   : This class handles the problem that often, when using a vector, we
           also have to know about matrices. For example, the expr.
           Vector a, b;
           SparseMatrix2D m;
           a = m * b;
           is valid, as the r.h.s. yields a MatrixVectorExpr<SparseMatrix2D, Vector>.
           The problem is that vector itself doesn't know about matrices and
           hence, we cannot define the operator in vector.h nor should we define it
           in matrix.h.
           The solution is for clients to not include vector.h and matrix.h, but to
           include this file, which in turn includes vector.h, matrix.h and defines
           necessary aperators.
 * Author: Sven Schmidt
 * Date  : 11/25/2011
 */
#pragma once

#include "DeclSpec.h"

#include "Vector.h"
#include "SparseMatrix2D.h"

#include "internal/MatrixVectorExpr.h"


namespace LinAlg_NS {

    // define operators

    LINALG_DECL_SYMBOLS Vector operator-(Vector const & lhs, Vector const & rhs);

    /* Note: Exporting a template function makes no sense.
     * Instead, we export the explicitly instantiated function in
     * the cpp file!
     */
    template<typename MATRIX_EXPR, typename VECTOR_EXPR>
    Vector operator-(Vector const & lhs, internal::MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR> const & rhs);

    LINALG_DECL_SYMBOLS Vector & operator-=(Vector & lhs, Vector const & rhs);

    LINALG_DECL_SYMBOLS Vector operator*(double value, Vector const & v);

    template<typename VECTOR_EXPR, typename BINOP>
    Vector & operator+=(Vector & lhs, internal::ScalarVectorBinaryExpr<VECTOR_EXPR, BINOP> const & rhs);

    LINALG_DECL_SYMBOLS Vector & operator+=(Vector & lhs, Vector const & rhs);

} // namespace LinAlg_NS
