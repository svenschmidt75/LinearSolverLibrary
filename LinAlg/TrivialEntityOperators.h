/*
 * Name  : TrivialEntityOperators
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

#ifndef ENTITY_OPERATORS
#error "Do not include TrivialEntityOperators.h directly, but EntityOperators.h instead"
#endif

#include "DeclSpec.h"

#include "Vector.h"


namespace LinAlg_NS {

    LINALG_DECL_SYMBOLS Vector & operator*=(Vector & lhs, double scalar);
    
    template<typename VECTOR_EXPR>
    Vector & operator+=(Vector & lhs, VECTOR_EXPR const & rhs);

    template<typename VECTOR_EXPR>
    Vector & operator-=(Vector & lhs, VECTOR_EXPR const & rhs);

} // namespace LinAlg_NS
