/*
 * Name  : entity_traits
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/28/2011
 */
#pragma once

#include <type_traits>

#include "helper.h"


namespace LinAlg_NS {

    // forward declarations
    class Vector;
    class Matrix2D;
    class SparseMatrix2D;

namespace internal {

    // forward-declarations
    template<typename T1, typename T2, typename BINOP>
    class VectorBinaryExpr;

    template<typename VECTOR_EXPR, typename BINOP>
    class ScalarVectorExpr;

    template<typename VECTOR_EXPR, typename BINOP>
    class ScalarVectorExpr;

    template<typename MATRIX_EXPR, typename BINOP>
    class ScalarMatrixExpr;

    template<typename MATRIX_EXPR, typename VECTOR_EXPR>
    class MatrixVectorExpr;

    template<typename MATRIX_EXPR_1, typename MATRIX_EXPR_2>
    class MatrixMatrixMul;

    //////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct entity_traits {
        enum { is_vector_expression = false };
        enum { is_matrix_expression = false };
    };

    template<>
    struct entity_traits<Vector> {
        enum { is_vector_expression = true };
        enum { is_matrix_expression = false };
    };

    template<typename VECTOR_EXPR, typename BINOP>
    struct entity_traits<ScalarVectorExpr<VECTOR_EXPR, BINOP>> {
        enum { is_vector_expression = true };
        enum { is_matrix_expression = false };
    };

    template<typename VECTOR_EXPR1, typename VECTOR_EXPR2, typename BINOP>
    struct entity_traits<VectorBinaryExpr<VECTOR_EXPR1, VECTOR_EXPR2, BINOP>> {
        enum { is_vector_expression = true };
        enum { is_matrix_expression = false };
    };

    template<typename MATRIX_EXPR, typename VECTOR_EXPR>
    struct entity_traits<MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR>> {
        enum { is_vector_expression = true };
        enum { is_matrix_expression = false };
    };

    //////////////////////////////////////////////////////////////////////////////

    template<>
    struct entity_traits<Matrix2D> {
        enum { is_vector_expression = false };
        enum { is_matrix_expression = true };
    };

    template<>
    struct entity_traits<SparseMatrix2D> {
        enum { is_vector_expression = false };
        enum { is_matrix_expression = true };
    };

    template<typename MATRIX_EXPR, typename BINOP>
    struct entity_traits<ScalarMatrixExpr<MATRIX_EXPR, BINOP>> {
        enum { is_vector_expression = false };
        enum { is_matrix_expression = true };
    };

    template<typename MATRIX_EXPR_1, typename MATRIX_EXPR_2>
    struct entity_traits<MatrixMatrixMul<MATRIX_EXPR_1, MATRIX_EXPR_2>> {
        enum { is_vector_expression = false };
        enum { is_matrix_expression = true };
    };

    //////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct matrix_operation_traits {};

    template<>
    struct matrix_operation_traits<Matrix2D> {

        template<typename VECTOR_EXPR>
        struct apply {
            /* Use tag dispatching to select the matrix multiplication
             * for dense matrices.
             */
            static double op(Matrix2D const & m, VECTOR_EXPR const & v, IMatrix2D::size_type row) {
                return LinAlg_NS::helper::matrix_vector_mul<VECTOR_EXPR>(m, v, row);
            }
        };

        static double get_value(Matrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col) {
            return LinAlg_NS::helper::get_value(m, row, col);
        }
    };

    template<>
    struct matrix_operation_traits<SparseMatrix2D> {

        template<typename VECTOR_EXPR>
        struct apply {
            static double op(SparseMatrix2D const & m, VECTOR_EXPR const & v, IMatrix2D::size_type row) {
                /* Use tag dispatching to select the matrix multiplication
                 * for sparse matrices.
                 */
                return LinAlg_NS::helper::matrix_vector_mul<VECTOR_EXPR>(m, v, row);
            }
        };

        static double get_value(SparseMatrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col) {
            return LinAlg_NS::helper::get_value(m, row, col);
        }
    };

    
    //////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct matrix_mul_traits {};

    template<>
    struct matrix_mul_traits<SparseMatrix2D> {
        using size_type = IMatrix2D::size_type;

        template<typename MATRIX_EXPR>
        struct apply {
            /* Use tag dispatching to select the matrix multiplication
             * for dense matrices.
             */
            static double op(SparseMatrix2D const & lhs, MATRIX_EXPR const & rhs, size_type row, size_type column) {
                /* Return
                 * result(row, column) = lhs(row, i) * rhs(i, column)
                 */
                return LinAlg_NS::helper::getMatrixMatrixMulElement<SparseMatrix2D, MATRIX_EXPR>(lhs, rhs, row, column);
            }
        };

    };


} // namespace internal

} // namespace LinAlg_NS
