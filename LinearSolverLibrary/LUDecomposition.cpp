#include "pch.h"

#include "LUDecomposition.h"



using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {


bool
LUDecomposition::decompose(LinAlg_NS::Matrix2D const & A) const {
    /* LU decomposition of square matrix A using Crout's method
     * with partial pivoting.
     * Note: the 1 on the diagonal is implicit, i.e. is not written
     * into LU_.
     */
    IMatrix2D::size_type max_row = A.rows();
    IMatrix2D::size_type max_col = A.cols();

    LU_.reset(new Matrix2D(A));

    initializePivoting(max_row);

    for (IMatrix2D::size_type col = 0; col < max_col; ++col) {
        LU_->print();
//         print(*LU_);

        auto physical_pivot_row_index = getPivotElementsRowIndex(*LU_, col);
        physical_pivot_row_index = col;

//        if (physical_pivot_row_index != col)
//            swapRows(col, physical_pivot_row_index);

        LU_->print();
//         print(*LU_);

        // "swap" rows 'col' and 'row_with_pivot_element' due to row pivoting
       adjustPivotingMap(col, physical_pivot_row_index);
        double pivot_element = (*LU_)(col, col);
        if (pivot_element == 0.0)
            // Matrix is singular
                return false;

        for (IMatrix2D::size_type i = col + 1; i < max_col; ++i) {
            (*LU_)(i, col) /= pivot_element;
            double ljk = (*LU_)(i, col);

            for (IMatrix2D::size_type j = col + 1; j < max_col; ++j) {
                (*LU_)(i, j) -= ljk * (*LU_)(col, j);
            }
        }

        LU_->print();
//         print(*LU_);
    }
    LU_->print();
//     print(*LU_);

//    rearrangeDueToPivoting();

    return true;
}

void
LUDecomposition::rearrangeDueToPivoting() const {
    decltype(partial_pivoting_map_) physical_map(partial_pivoting_map_.size());
    for (auto i = 0; i < partial_pivoting_map_.size(); ++i) {
        physical_map[i] = physicalToLogicalRowIndex(i);
    }

    for (auto i = 0; i < physical_map.size(); ++i) {
        auto j = std::distance(std::begin(physical_map), std::find(std::begin(physical_map), std::end(physical_map), i));
        if (i == j)
            continue;

        // Swap rows
        for (auto col = 0; col < LU_->cols(); ++col)
            std::swap((*LU_)(i, col), (*LU_)(j, col));

//         AInverse.print();
//         print(AInverse);
    }

//     AInverse.print();
//     print(AInverse);
}

void
LUDecomposition::swapRows(IMatrix2D::size_type row1, IMatrix2D::size_type row2) const {
    for (auto col = 0; col < LU_->cols(); ++col)
        std::swap((*LU_)(row1, col), (*LU_)(row2, col));
}

void
LUDecomposition::initializePivoting(IMatrix2D::size_type rows) const {
    partial_pivoting_map_.resize(rows);
    std::iota(std::begin(partial_pivoting_map_), std::end(partial_pivoting_map_), 0ull);
}

IMatrix2D::size_type
LUDecomposition::getPivotElementsRowIndex(Matrix2D const & A, IMatrix2D::size_type column_index) const {
    /* Return the largest element in the column.
     * Note that no row can be pivot row more than once.
     * This is 
     */
    IMatrix2D::size_type max_row = A.rows();
    IMatrix2D::size_type pivot_index = 0;
    double pivot_value = 0;
    double val;
    for (auto row_index = column_index; row_index < max_row; ++row_index) {
        auto mapped_row_index = logicalToPhysicalRowIndex(row_index);
        val = std::fabs(A(mapped_row_index, column_index));
        if (val > pivot_value) {
            pivot_index = mapped_row_index;
            pivot_value = val;
        }
    }
    return pivot_index;
}

void
LUDecomposition::adjustPivotingMap(IMatrix2D::size_type source_row, IMatrix2D::size_type dest_row) const {
    /* This maps logical rows (i.e. rows with pivoting) to physical
     * rows of the matrix.
     * Swaps rows of necessary.
     */
    if (dest_row != partial_pivoting_map_[source_row]) {
        auto index = partial_pivoting_map_[dest_row];
        std::swap(partial_pivoting_map_[source_row], partial_pivoting_map_[index]);
    }
}

void
LUDecomposition::print(Matrix2D const & A) const {
    std::cout << std::endl;
    for (IMatrix2D::size_type i = 0; i < A.rows(); ++i) {
        IMatrix2D::size_type row = logicalToPhysicalRowIndex(i);
        for (IMatrix2D::size_type j = 0; j < A.cols(); ++j) {
            std::cout << std::setw(10) << A(row, j);
        }
        std::cout << std::endl;
    }
}

IMatrix2D::size_type
LUDecomposition::logicalToPhysicalRowIndex(IMatrix2D::size_type logical_row_index) const {
    BOOST_ASSERT_MSG(logical_row_index < partial_pivoting_map_.size(), "Gauss::logicalToPhysicalRowIndex: Index out of range");
    // maps the row index with pivoting to the one without pivoting
    IMatrix2D::size_type physical_row_index = partial_pivoting_map_[logical_row_index];
    return physical_row_index;
}

IMatrix2D::size_type
LUDecomposition::physicalToLogicalRowIndex(IMatrix2D::size_type physical_row_index) const {
    BOOST_ASSERT_MSG(physical_row_index < partial_pivoting_map_.size(), "Gauss::physicalToLogicalRowIndex: Index out of range");
    // maps the row index into its row index with pivoting
    IMatrix2D::size_type logical_row_index = std::distance(std::begin(partial_pivoting_map_), std::find(std::begin(partial_pivoting_map_), std::end(partial_pivoting_map_), physical_row_index));
    return logical_row_index;
}

// LinAlg_NS::Vector
// LUDecomposition::solve(LinAlg_NS::Vector const & rhs) const {
// }

} // LinearSolverLibrary_NS
