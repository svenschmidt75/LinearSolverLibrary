#include "pch.h"

#include "LUDecomposition.h"



using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {


bool
LUDecomposition::decompose(LinAlg_NS::Matrix2D const & A) const {
    /* LU decomposition of square matrix A using Crout's method
     * with partial pivoting.
     */
    IMatrix2D::size_type max_row = A.rows();
    IMatrix2D::size_type max_col = A.cols();

    LU_.reset(new Matrix2D(A));

    initializePivoting(max_row);

    for (IMatrix2D::size_type col = 0; col < max_col; ++col) {
//         ACopy.print();
//        print(ACopy);
//         AInverse.print();
//        print(ACopy);

        auto physical_pivot_row_index = getPivotElementsRowIndex(*LU_, col);
        physical_pivot_row_index = col;

        // "swap" rows 'col' and 'row_with_pivot_element' due to row pivoting
        adjustPivotingMap(col, physical_pivot_row_index);
        double pivot_element = (*LU_)(physical_pivot_row_index, col);
        if (pivot_element == 0.0)
            // Matrix is singular
                return false;

        // Divide pivot row by pivot element to set pivot element to 1
        // as part of the reduction of ACopy to the identity matrix.
        // Note: Start column is 'col', as all elements with column
        // index (0, ..., col - 1) are 0 already.
        for (IMatrix2D::size_type i = 0; i < max_col; ++i) {
            if (i >= col) {
                double & val1 = (*LU_)(physical_pivot_row_index, i);
                val1 /= pivot_element;
            }
        }

//         ACopy.print();
//        print(ACopy);
//         AInverse.print();

        // Add pivot row to all later rows such that all elements
        // in those rows become 0, i.e. we set the column to the
        // column of the identity matrix.
        for (IMatrix2D::size_type i = 0; i < max_row; ++i) {
            auto mapped_i = logicalToPhysicalRowIndex(i);
            if (mapped_i == physical_pivot_row_index)
                // skip pivot row
                    continue;
            double val = - (*LU_)(mapped_i, col) / (*LU_)(physical_pivot_row_index, col);

            // subtract the pivot row from row 'mapped_i'
            for (IMatrix2D::size_type j = 0; j < max_col; ++j) {
                if (j >= col)
                    (*LU_)(mapped_i, j) += val * (*LU_)(physical_pivot_row_index, j);
            }

//             ACopy.print();
//            print(ACopy);
//             AInverse.print();
        }

//         ACopy.print();
//        print(ACopy);
//         AInverse.print();
    }

//     AInverse.print();
//     print(AInverse);





    return true;
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
