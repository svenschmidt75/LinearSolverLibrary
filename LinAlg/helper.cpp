#include "pch.h"

#include "helper.h"
#include "Vector.h"


namespace LinAlg_NS {

SparseMatrix2D
helper::transpose(SparseMatrix2D const & m) {
    /* Transpose a sparse matrix. My first idea was to write a wrapper that behaves like the transpose,
     * but when multiplying this wrapper with a vector, we end up doing a dense matrix multiplication.
     * Hence this explicit construction.
     */
    SparseMatrix2D transposed(m.cols(), m.rows());
    for (SparseMatrix2D::size_type row = 0; row < m.rows(); ++row) {
        // Number of non-zero columns for this row
        IMatrix2D::size_type ncol = m.nelements_[row + 1] - m.nelements_[row];
        IMatrix2D::size_type offset = m.nelements_[row];

        // all non-zero columns
        for (int icol = 0; icol < ncol; ++icol) {
            IMatrix2D::size_type col = m.columns_[offset + icol];
            double a_ij = m.elements_[offset + icol];
            transposed(col, row) = a_ij;
        }
    }
    transposed.finalize();
    return transposed;
}

template<typename MATRIX_EXPR>
double
helper::get_value(MATRIX_EXPR const & m, IMatrix2D::size_type row, IMatrix2D::size_type col) {
    double value = m(row, col);
    return value;
}

bool
helper::isSymmetric(SparseMatrix2D const & m) {
    if (m.rows() != m.cols())
        return false;
    /* Access pattern of algorithm:
     * 
     *       |             ...
     *       | m(i,0) ... m(i,i) m{i,i+1} ... m(i,cols)|
     *       |             ...
     *       | ...        m(j,k)    ...
     *       | ...        m(j+1,k)  ...
     *       | ...         ...      ...
     *       | ...        m(rows,k) ...
     * 
     * For each row i, all elements a(i,k) with k = i + 1 ... cols
     * (i.e. to the right of the diagonal element m(i,i)) are compared
     * to the elements in column i, a(k,i).
     * Thus, the access pattern is like this:
     * 
     * 
     * 
     * 
     * 
     *              ith column
     *                 \/
     *      ith row: >  ************************
     *                  * ...
     *                  * ...
     *                  * ...
     *                  * ...
     *                  * ...
     *                  * ...
     *                  * ...
     */
    using size_type = IMatrix2D::size_type;
    static double const tol = 1E-10;

    // m.rows() + 1, because the last for the last row, there is
    // only the diagonal element a(m.rows() - 1, m.rows() - 1),
    // so nothink to check.
    for (size_type row{0}; row < m.rows() - 1; ++row) {
        for (size_type col{row + 1}; col < m.cols(); ++col) {
            double a_ij = m(row, col);
            double a_ji = m(col, row);
            double delta = std::fabs(a_ij - a_ji);
            if (delta > tol)
                return false;
        }
    }
    return true;
}

SparseMatrix2D
helper::matrixMul(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs) {
    common_NS::reporting::checkConditional(lhs.cols() == rhs.rows(), "helper::matrixMul: Matrices incompatible");
    auto nrows = lhs.rows();
    auto ncols = rhs.cols();
    SparseMatrix2D tmp{nrows, ncols};
    for (IMatrix2D::size_type row = 0; row < nrows; ++row) {
        auto value = 0.0;
        ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(lhs, row);
        for (IMatrix2D::size_type column = 0; column < ncols; ++column) {
            ConstColumnRowIterator<SparseMatrix2D> rowColumnIterator = MatrixIterators::getConstColumnRowIterator(rhs, column);
            ConstColumnIterator<SparseMatrix2D> columnIterator = *columnRowIterator;
            ConstRowIterator<SparseMatrix2D> rowIterator = *rowColumnIterator;

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
            if (value) {
                tmp(row, column) = value;
                value = 0.0;
            }
        }
    }
    tmp.finalize();
    return tmp;
}

// create explicit template instantiations
template LINALG_DECL_SYMBOLS double helper::get_value(Matrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);
template LINALG_DECL_SYMBOLS double helper::get_value(SparseMatrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);

} // namespace LinAlg_NS
