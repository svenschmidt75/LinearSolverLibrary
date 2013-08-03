#include "pch.h"

#include "Gauss.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

Gauss::Return_t
Gauss::solve(Matrix2D const & A, Vector const & f) {
    // Solve the equation A x = f using Gauss' elimination with row pivoting.
    // Description of algorithm: First, search for the largest element
    // in all following rows and use it as the pivot element. This is to
    // reduce round-off errors. The matrix ACopy will be transformed
    // into the identity matrix, while the identity matrix, AInv, will
    // be transformed into the inverse.
    // A x =  Id y is successively transformed into A^{-1} A x = A^{-1} Id f,
    // where ACopy = Id and A^{-1} = AInv.

    BOOST_ASSERT_MSG(A.cols() == A.rows(), "Gauss::solve: Matrix must be quadratic");
    BOOST_ASSERT_MSG(A.cols() == f.size(), "Gauss::solve: r.h.s. vector size mismatch");

    IMatrix2D::size_type max_col = A.cols();
    IMatrix2D::size_type max_row = A.rows();
    bool success = false;
    Matrix2D ACopy(A);
    Matrix2D AInverse(Matrix2D::identity(max_col));
    Vector rhs(f);

    initializePartialPivotingMap(max_row);

    for (IMatrix2D::size_type col = 0; col < max_col; ++col) {
        // Find pivot element row index
        IMatrix2D::size_type pivot_index = findPivotIndex(ACopy, col);

        // Arrange rows due to row pivoting
        adjustPivotingMap(col, pivot_index);
        double pivot_element = ACopy(pivot_index, col);

        // Matrix is singular
        if (pivot_element == 0.0)
            return std::make_tuple(false, AInverse, rhs);

        // Divide pivot row by pivot element to set element to 1
        for (IMatrix2D::size_type i = 0; i < max_col; ++i) {
            // ACopy(pivot_index, i) = 0 for i < col
            if (i >= col) {
                double & val1 = ACopy(pivot_index, i);
                val1 /= pivot_element;
            }
            double & val2 = AInverse(pivot_index, i);
            val2 /= pivot_element;
        }

        // Do same transformation on the rhs
        rhs(pivot_index) /= pivot_element;

        // Add pivot row to all other rows to reduce column col
        // to the corresponding identity matrix column.
        for (int current_row = 0; current_row < max_row; ++current_row) {
            if (current_row == pivot_index)
                continue;

            double val = - ACopy(current_row, col) / ACopy(pivot_index, col);

            for (int j = 0; j < max_col; ++j) {
                ACopy(current_row, j) += val * ACopy(pivot_index, j);
                AInverse(current_row, j) += val * AInverse(pivot_index, j);
            }

            // Do same transformation on the rhs
            rhs(current_row) += val * rhs(pivot_index);
        }
    }

    // Rearrange the rows in AInverse due to pivoting
    rearrangeDueToPivoting(ACopy, AInverse, rhs);

    return std::make_tuple(success, AInverse, rhs);
}

void
Gauss::initializePartialPivotingMap(IMatrix2D::size_type rows) const {
    partial_pivoting_map_.resize(rows);
    std::iota(std::begin(partial_pivoting_map_), std::end(partial_pivoting_map_), 0ull);
}

IMatrix2D::size_type
Gauss::findPivotIndex(Matrix2D const & A, IMatrix2D::size_type column_index) {
    IMatrix2D::size_type max_row = A.rows();
    IMatrix2D::size_type pivot_index = 0;
    double pivot_value = std::numeric_limits<double>::min();
    double val;
    for (IMatrix2D::size_type row = 0; row < max_row; ++row) {
        IMatrix2D::size_type mapped_row = partial_pivoting_map_[row];
        if (mapped_row < column_index)
            continue;
        val = std::fabs(A(mapped_row, column_index));
        if (val > pivot_value) {
            pivot_index = row;
            pivot_value = val;
        }
    }
    return pivot_index;
}

void
Gauss::adjustPivotingMap(IMatrix2D::size_type column_index, IMatrix2D::size_type pivot_index) const {
    if (column_index != partial_pivoting_map_[pivot_index]) {
        partial_pivoting_map_[pivot_index] = column_index;
        partial_pivoting_map_[partial_pivoting_map_[column_index]] = pivot_index;
    }
    BOOST_ASSERT_MSG(partial_pivoting_map_[pivot_index] == column_index, "Gauss::adjustPivotingMap: Pivoting error");
}

void
Gauss::rearrangeDueToPivoting(Matrix2D & A, Matrix2D & AInverse, Vector & rhs) const {
    for (IMatrix2D::size_type i = 0; i < A.rows(); ++i) {
        IMatrix2D::size_type row = partial_pivoting_map_[i];
        if (row == i)
            continue;

        // Swap rows
        for (IMatrix2D::size_type j = 0; j < A.cols(); ++j) {
            double tmp = AInverse(i, j);
            AInverse(i, j) = AInverse(row, j);
            AInverse(row, j) = tmp;

            tmp = A(i, j);
            A(i, j) = A(row, j);
            A(row, j) = tmp;
        }

        // Swap rows on the rhs
        double tmp = rhs(i);
        rhs(i) = rhs(row);
        rhs(row) = tmp;

        partial_pivoting_map_[i] = partial_pivoting_map_[row];
        partial_pivoting_map_[row] = row;
    }
}

} // LinearSolverLibrary_NS
