#include "pch.h"

#include "GaussJordan.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

GaussJordan::Return_t
GaussJordan::solve(Matrix2D const & A, Vector const & f) const {
    // Solve the equation A x = f using Gauss-Jordan elimination with row pivoting.
    // Description of algorithm: First, search for the largest element in the current
    // column use it as the pivot element. This is to reduce round-off errors. The
    // matrix ACopy will be transformed into the identity matrix, while the identity
    // matrix, AInvwerse, will be transformed into the inverse.
    // A x =  Id y is successively transformed into A^{-1} A x = A^{-1} Id f,
    // where ACopy = Id and A^{-1} = AInverse.

    BOOST_ASSERT_MSG(A.cols() == A.rows(), "Gauss::solve: Matrix must be quadratic");
    BOOST_ASSERT_MSG(A.cols() == f.size(), "Gauss::solve: r.h.s. vector size mismatch");

    IMatrix2D::size_type max_col = A.cols();
    IMatrix2D::size_type max_row = A.rows();
    bool success = false;
    Matrix2D ACopy(A);
    Matrix2D AInverse(Matrix2D::identity(max_col));
    Vector rhs(f);

    initializePivoting(max_row);

    /* This is a column index, rather than a row index.
     * Obviously, this makes no difference for a square
     * matrix, but if #columns > #rows, the problem is
     * overspecified and then least-squares could be used.
     * The other extreme is an underspecified problem, which
     * we do not bother with at all here.
     */
    for (IMatrix2D::size_type col = 0; col < max_col; ++col) {
//         ACopy.print();
//        print(ACopy);
//         AInverse.print();
//        print(ACopy);

        auto physical_pivot_row_index = getPivotElementsRowIndex(ACopy, col);

        // "swap" rows 'col' and 'row_with_pivot_element' due to row pivoting
        adjustPivotingMap(col, physical_pivot_row_index);
        double pivot_element = ACopy(physical_pivot_row_index, col);
        if (pivot_element == 0.0)
            // Matrix is singular
            return std::make_tuple(false, AInverse, rhs);

        // Divide pivot row by pivot element to set pivot element to 1
        // as part of the reduction of ACopy to the identity matrix.
        // Note: Start column is 'col', as all elements with column
        // index (0, ..., col - 1) are 0 already.
        for (IMatrix2D::size_type i = 0; i < max_col; ++i) {
            if (i >= col) {
                double & val1 = ACopy(physical_pivot_row_index, i);
                val1 /= pivot_element;
            }
            double & val2 = AInverse(physical_pivot_row_index, i);
            val2 /= pivot_element;
        }

//         ACopy.print();
//        print(ACopy);
//         AInverse.print();

        // Do same transformation on the rhs
        rhs(physical_pivot_row_index) /= pivot_element;

        // Add pivot row to all later rows such that all elements
        // in those rows become 0, i.e. we set the column to the
        // column of the identity matrix.
        for (IMatrix2D::size_type i = 0; i < max_row; ++i) {
            auto mapped_i = logicalToPhysicalRowIndex(i);
            if (mapped_i == physical_pivot_row_index)
                // skip pivot row
                continue;
            double val = - ACopy(mapped_i, col) / ACopy(physical_pivot_row_index, col);

            // subtract the pivot row from row 'mapped_i'
            for (IMatrix2D::size_type j = 0; j < max_col; ++j) {
                if (j >= col)
                    ACopy(mapped_i, j) += val * ACopy(physical_pivot_row_index, j);
                AInverse(mapped_i, j) += val * AInverse(physical_pivot_row_index, j);
            }

            // do same transformation on the rhs
            rhs(mapped_i) += val * rhs(physical_pivot_row_index);

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

    // Rearrange the rows in AInverse due to pivoting
    rearrangeDueToPivoting(ACopy, AInverse, rhs);

//     ACopy.print();
//     AInverse.print();

    return std::make_tuple(success, AInverse, rhs);
}

void
GaussJordan::initializePivoting(IMatrix2D::size_type rows) const {
    partial_pivoting_map_.resize(rows);
    std::iota(std::begin(partial_pivoting_map_), std::end(partial_pivoting_map_), 0ull);
}

IMatrix2D::size_type
GaussJordan::getPivotElementsRowIndex(Matrix2D const & A, IMatrix2D::size_type column_index) const {
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
GaussJordan::adjustPivotingMap(IMatrix2D::size_type source_row, IMatrix2D::size_type dest_row) const {
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
GaussJordan::rearrangeDueToPivoting(Matrix2D & A, Matrix2D & AInverse, Vector & rhs) const {
    decltype(partial_pivoting_map_) physical_map(partial_pivoting_map_.size());
    for (auto i = 0; i < partial_pivoting_map_.size(); ++i) {
        physical_map[i] = physicalToLogicalRowIndex(i);
    }

    for (auto i = 0; i < physical_map.size(); ++i) {
        auto j = std::distance(std::begin(physical_map), std::find(std::begin(physical_map), std::end(physical_map), i));
        if (i == j)
            continue;

        // Swap rows
        for (auto col = 0; col < A.cols(); ++col) {
            std::swap(AInverse(i, col), AInverse(j, col));
            std::swap(A(i, col), A(j, col));
        }

//         AInverse.print();
//         print(AInverse);

        // Swap rows on the r.h.s.
        std::swap(rhs(i), rhs(j));
        std::swap(physical_map[i], physical_map[j]);
    }

//     AInverse.print();
//     print(AInverse);
}

void
GaussJordan::print(Matrix2D const & A) const {
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
GaussJordan::logicalToPhysicalRowIndex(IMatrix2D::size_type logical_row_index) const {
    BOOST_ASSERT_MSG(logical_row_index < partial_pivoting_map_.size(), "Gauss::logicalToPhysicalRowIndex: Index out of range");
    // maps the row index with pivoting to the one without pivoting
    IMatrix2D::size_type physical_row_index = partial_pivoting_map_[logical_row_index];
    return physical_row_index;
}

IMatrix2D::size_type
GaussJordan::physicalToLogicalRowIndex(IMatrix2D::size_type physical_row_index) const {
    BOOST_ASSERT_MSG(physical_row_index < partial_pivoting_map_.size(), "Gauss::physicalToLogicalRowIndex: Index out of range");
    // maps the row index into its row index with pivoting
    IMatrix2D::size_type logical_row_index = std::distance(std::begin(partial_pivoting_map_), std::find(std::begin(partial_pivoting_map_), std::end(partial_pivoting_map_), physical_row_index));
    return logical_row_index;
}

} // LinearSolverLibrary_NS
