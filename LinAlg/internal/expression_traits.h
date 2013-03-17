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
    class SparseMatrix2D;

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

    template<typename MATRIX_EXPR, typename VECTOR_EXPR>
    class MatrixVectorExpr;

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
    };

    template<>
    struct expression_traits<SparseMatrix2D> {
        typedef std::true_type is_matrix_expression;
    };

    template<typename MATRIX_EXPR, typename BINOP>
    struct expression_traits<ScalarMatrixExpr<MATRIX_EXPR, BINOP>> {
        typedef std::true_type is_matrix_expression;
    };

    template<typename MATRIX_EXPR, typename VECTOR_EXPR>
    struct expression_traits<MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR>> {
        typedef std::true_type is_vector_expression;
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

} // namespace internal

} // namespace LinAlg_NS
