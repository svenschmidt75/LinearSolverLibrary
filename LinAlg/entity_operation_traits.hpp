/*
 * Name  : entity_operation_traits
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 12/28/2013
 */
#pragma once

#include "helper.h"


namespace LinAlg_NS {

    // forward declarations
    class Vector;
    class Matrix2D;
    class SparseMatrix2D;

    namespace internal {


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
                return LinAlg_NS::helper::getMatrixMatrixMulElement(lhs, rhs, row, column);
            }
        };

    };

    template<typename T1, typename T2>
    struct matrix_mul_traits<MatrixMatrixMul<T1, T2>> {
        using size_type = IMatrix2D::size_type;

        template<typename MATRIX_EXPR>
        struct apply {
            /* Use tag dispatching to select the matrix multiplication
             * for dense matrices.
             */
            static double op(MatrixMatrixMul<T1, T2> const & lhs, MATRIX_EXPR const & rhs, size_type row, size_type column) {
                /* Return
                 * result(row, column) = lhs(row, i) * rhs(i, column)
                 */
                return LinAlg_NS::helper::getMatrixMatrixMulElement(lhs, rhs, row, column);
            }
        };

    };

    } // namespace internal

} // namespace LinAlg_NS
