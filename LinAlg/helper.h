/*
 * Name  : Helper
 * Path  : 
 * Use   : Implements helper functions for objects defined
 *         in LinAlg.
 * Author: Sven Schmidt
 * Date  : 07/29/2011
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"
#include "Matrix2D.h"
#include "SparseMatrix2D.h"
#include "MatrixIterators.h"
#include "ConstColumnRowIterator.h"
#include "ConstRowColumnIterator.h"
#include "ConstRowIterator.h"
#include "ConstColumnIterator.h"
#include "ConstRowIteratorGeneric.hpp"
#include "ConstColumnRowIteratorGeneric.hpp"


// forward declarations
class ParallelLinAlgOperationsTest;


namespace LinAlg_NS {

    class Vector;
    class TransposeVector;
    class Matrix2D;
    class SparseMatrix2D;


    class LINALG_DECL_SYMBOLS helper {


        friend class ParallelLinAlgOperationsTest;


    public:
        using size_type = IMatrix2D::size_type;

    public:
        static TransposeVector transpose(Vector const & vec);
        static Vector          transpose(TransposeVector const & vec);
        static SparseMatrix2D  transpose(SparseMatrix2D const & m);

        template<typename MATRIX_EXPR>
        static double
        get_value(MATRIX_EXPR const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);

        template<typename VECTOR_EXPR>
        static double
        matrix_vector_mul(SparseMatrix2D const & m, VECTOR_EXPR const & v, IMatrix2D::size_type row) {
            /* Multiply one row of the sparse matrix with
             * the vector.
             */
#ifdef _DEBUG
            common_NS::reporting::checkConditional(m.cols() == v.size(), "MatrixVectorExpr: Matrix and vector incompatible");
#endif
            // Number of non-zero columns for this row
            IMatrix2D::size_type ncol = m.columns_offset_[row + 1] - m.columns_offset_[row];
            IMatrix2D::size_type offset = m.columns_offset_[row];

            double value = 0;

            // all non-zero columns
            for (int icol = 0; icol < ncol; ++icol) {
                IMatrix2D::size_type col = m.columns_[offset + icol];
                double a_ij = m.elements_[offset + icol];
                value += (a_ij * v(col));
            }
            return value;
        }

        template<typename VECTOR_EXPR>
        static double
        matrix_vector_mul(Matrix2D const & m, VECTOR_EXPR const & v, IMatrix2D::size_type row) {
            /* Multiply one row of the sparse matrix with
             * the vector.
             */
#ifdef _DEBUG
            common_NS::reporting::checkConditional(m.cols() == v.size(), "MatrixVectorExpr: Matrix and vector incompatible");
#endif
            double value = 0;
            for (IMatrix2D::size_type col = 0; col < m.cols(); ++col) {
                double a_ij = m(row, col);
                value += (a_ij * v(col));
            }
            return value;
        }

        static bool isSymmetric(SparseMatrix2D const & m);

        static SparseMatrix2D matrixMul(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs);
        static SparseMatrix2D matrixMulSerial(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs);
        static SparseMatrix2D nonChunkedParallelMatrixMatrixMultiplication(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs);
        static SparseMatrix2D chunkedParallelMatrixMatrixMultiplication(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs);
        static Matrix2D       matrixMul(Matrix2D const & lhs, Matrix2D const & rhs);
        static Matrix2D       matrixMulSerial(Matrix2D const & lhs, Matrix2D const & rhs);

        template<typename MATRIX_EXPR_1, typename MATRIX_EXPR_2>
        static double getMatrixMatrixMulElement(MATRIX_EXPR_1 const & lhs, MATRIX_EXPR_2 const & rhs, size_type row, size_type column) {
#ifdef _DEBUG
            common_NS::reporting::checkConditional(lhs.cols() == rhs.rows(), "helper::matrix_matrix_mul: Matrices incompatible");
#endif
            auto value = 0.0;
            ConstRowColumnIterator<MATRIX_EXPR_1> columnRowIterator = MatrixIterators::getConstRowColumnIterator(lhs, row);
            ConstColumnRowIterator<MATRIX_EXPR_2> rowColumnIterator = MatrixIterators::getConstColumnRowIterator(rhs, column);
            ConstColumnIterator<MATRIX_EXPR_1> columnIterator = *columnRowIterator;
            ConstRowIterator<MATRIX_EXPR_2> rowIterator = *rowColumnIterator;

            // 1st element in row'th row of lhs: lhs(row, columnIterator.column())
            while (columnIterator && columnIterator.column() < lhs.cols()) {
                while (rowIterator && rowIterator.row() < columnIterator.column()) {
                    ++rowIterator;
                }
                if (!rowIterator)
                    break;
                if (rowIterator.row() == columnIterator.column())
                    value += lhs(row, rowIterator.row()) * rhs(rowIterator.row(), column);
                ++columnIterator;
            }

            return value;
        }

        static Matrix2D       SparseToDense(SparseMatrix2D const & sparse);
        static SparseMatrix2D DenseToSparse(Matrix2D const & dense);

    private:
        static SparseMatrix2D  transposeSerial(SparseMatrix2D const & m);
        static SparseMatrix2D  transposeParallelNonChunked(SparseMatrix2D const & m);
        static SparseMatrix2D  transposeParallelChunked(SparseMatrix2D const & m);

        static bool isSymmetricSerial(SparseMatrix2D const & m);
        static bool matrixIsSymmetricParallelNonChunked(SparseMatrix2D const & m);
        static bool matrixIsSymmetricParallelChunked(SparseMatrix2D const & m);
    };

} // namespace LinAlg_NS
