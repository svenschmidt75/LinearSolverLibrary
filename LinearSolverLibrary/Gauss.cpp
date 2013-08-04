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
        ACopy.print();
        print(ACopy);

        // Find pivot element row index
        auto physical_pivot_row_index = findPivotRowIndex(ACopy, col);

        // "swap" rows 'col' and 'row_with_pivot_element' due to row pivoting
        adjustPivotingMap(col, physical_pivot_row_index);
        double pivot_element = ACopy(physical_pivot_row_index, col);

        // Matrix is singular
        if (pivot_element == 0.0)
            return std::make_tuple(false, AInverse, rhs);


        // SCALE ROW



        // divide pivot row by pivot element to set pivot element to 1
        for (IMatrix2D::size_type i = 0; i < max_col; ++i) {
            // ACopy(pivot_index, i) = 0 for i < col
            if (i >= col) {
                double & val1 = ACopy(physical_pivot_row_index, i);
                val1 /= pivot_element;
            }
            double & val2 = AInverse(physical_pivot_row_index, i);
            val2 /= pivot_element;
        }

        ACopy.print();
        print(ACopy);

        // Do same transformation on the rhs
        rhs(physical_pivot_row_index) /= pivot_element;



        // SET TO ZERO



        // Add pivot row to all other rows to reduce column col
        // to the corresponding identity matrix column.
        for (IMatrix2D::size_type current_row = 0; current_row < max_row; ++current_row) {
            if (current_row == physical_pivot_row_index)
                continue;

            double val = - ACopy(current_row, col) / ACopy(physical_pivot_row_index, col);

            for (IMatrix2D::size_type j = 0; j < max_col; ++j) {
                ACopy(current_row, j) += val * ACopy(physical_pivot_row_index, j);
                AInverse(current_row, j) += val * AInverse(physical_pivot_row_index, j);
            }

            // do same transformation on the rhs
            rhs(current_row) += val * rhs(physical_pivot_row_index);
        }

        ACopy.print();
        print(ACopy);
    }

    AInverse.print();
    print(AInverse);

    // Rearrange the rows in AInverse due to pivoting
    rearrangeDueToPivoting(ACopy, AInverse, rhs);

    ACopy.print();
    AInverse.print();

    return std::make_tuple(success, AInverse, rhs);
}

void
Gauss::initializePartialPivotingMap(IMatrix2D::size_type rows) const {
    partial_pivoting_map_.resize(rows);
    std::iota(std::begin(partial_pivoting_map_), std::end(partial_pivoting_map_), 0ull);
}

IMatrix2D::size_type
Gauss::findPivotRowIndex(Matrix2D const & A, IMatrix2D::size_type column_index) {
    IMatrix2D::size_type max_row = A.rows();
    IMatrix2D::size_type pivot_index = 0;
    double pivot_value = std::numeric_limits<double>::min();
    double val;
    for (IMatrix2D::size_type physical_row_index = 0; physical_row_index < max_row; ++physical_row_index) {
        IMatrix2D::size_type logical_row_index = physicalToLogicalRowIndex(physical_row_index);
        if (logical_row_index < column_index)
            continue;
        val = std::fabs(A(physical_row_index, column_index));
        if (val > pivot_value) {
            pivot_index = physical_row_index;
            pivot_value = val;
        }
    }
    return pivot_index;
}

void
Gauss::adjustPivotingMap(IMatrix2D::size_type source_row, IMatrix2D::size_type dest_row) const {
    // "swap" rows source_row to dest_row and vice versa
//    BOOST_ASSERT_MSG(partial_pivoting_map_[pivot_index] == column_index, "Gauss::adjustPivotingMap: Pivoting error");
    // dest_row must be > than source_row!!!

    IMatrix2D::size_type logical_source_row_index = source_row;
    IMatrix2D::size_type logical_dest_row_index = physicalToLogicalRowIndex(dest_row);
    IMatrix2D::size_type tmp = partial_pivoting_map_[logical_source_row_index];
    partial_pivoting_map_[logical_source_row_index] = partial_pivoting_map_[logical_dest_row_index];
    partial_pivoting_map_[logical_dest_row_index] = tmp;
}

void
Gauss::rearrangeDueToPivoting(Matrix2D & A, Matrix2D & AInverse, Vector & rhs) const {
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
            double tmp = AInverse(i, col);
            AInverse(i, col) = AInverse(j, col);
            AInverse(j, col) = tmp;

            tmp = A(i, col);
            A(i, col) = A(j, col);
            A(j, col) = tmp;
        }

        AInverse.print();
        print(AInverse);

        // Swap rows on the r.h.s.
        double tmp2 = rhs(i);
        rhs(i) = rhs(j);
        rhs(j) = tmp2;

        auto tmp = physical_map[i];
        physical_map[i] = i;
        physical_map[j] = tmp;
    }

    AInverse.print();
    print(AInverse);
}

void
Gauss::print(Matrix2D const & A) const {
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
Gauss::logicalToPhysicalRowIndex(IMatrix2D::size_type logical_row_index) const {
    BOOST_ASSERT_MSG(logical_row_index < partial_pivoting_map_.size(), "Gauss::logicalToPhysicalRowIndex: Index out of range");
    // maps the row index with pivoting to the one without pivoting
    IMatrix2D::size_type physical_row_index = partial_pivoting_map_[logical_row_index];
    return physical_row_index;
}

IMatrix2D::size_type
Gauss::physicalToLogicalRowIndex(IMatrix2D::size_type physical_row_index) const {
    BOOST_ASSERT_MSG(physical_row_index < partial_pivoting_map_.size(), "Gauss::physicalToLogicalRowIndex: Index out of range");
    // maps the row index into its row index with pivoting
    IMatrix2D::size_type logical_row_index = std::distance(std::begin(partial_pivoting_map_), std::find(std::begin(partial_pivoting_map_), std::end(partial_pivoting_map_), physical_row_index));
    return logical_row_index;
}

} // LinearSolverLibrary_NS
