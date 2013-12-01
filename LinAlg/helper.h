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

#include "common/reporting.h"

#include <stdexcept>

#include <boost/assert.hpp>


namespace LinAlg_NS {

    class Vector;
    class TransposeVector;
    class Matrix2D;
    class SparseMatrix2D;


    class LINALG_DECL_SYMBOLS helper {
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
#ifdef DEBUG
            common_NS::reporting::checkConditional(m.cols() == v.size(), "MatrixVectorExpr: Matrix and vector incompatible");
#endif
            // Number of non-zero columns for this row
            IMatrix2D::size_type ncol = m.nelements_[row + 1] - m.nelements_[row];
            IMatrix2D::size_type offset = m.nelements_[row];

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
#ifdef DEBUG
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
    };

} // namespace LinAlg_NS
