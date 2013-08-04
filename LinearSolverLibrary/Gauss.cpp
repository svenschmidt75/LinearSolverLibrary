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
        IMatrix2D::size_type row_with_pivot_element = findRowWithPivotElement(ACopy, col);

        // "swap" rows 'col' and 'row_with_pivot_element' due to row pivoting
        adjustPivotingMap(col, row_with_pivot_element);
        double pivot_element = ACopy(row_with_pivot_element, col);

        // Matrix is singular
        if (pivot_element == 0.0)
            return std::make_tuple(false, AInverse, rhs);


        // SCALE ROW



        // divide pivot row by pivot element to set pivot element to 1
        for (IMatrix2D::size_type i = 0; i < max_col; ++i) {
            // ACopy(pivot_index, i) = 0 for i < col
            if (i >= col) {
                double & val1 = ACopy(row_with_pivot_element, i);
                val1 /= pivot_element;
            }
            double & val2 = AInverse(row_with_pivot_element, i);
            val2 /= pivot_element;
        }

        ACopy.print();
        print(ACopy);

        // Do same transformation on the rhs
        rhs(row_with_pivot_element) /= pivot_element;



        // SET TO ZERO



        // Add pivot row to all other rows to reduce column col
        // to the corresponding identity matrix column.
        for (IMatrix2D::size_type current_row = 0; current_row < max_row; ++current_row) {
            if (current_row == row_with_pivot_element)
                continue;

            double val = - ACopy(current_row, col) / ACopy(row_with_pivot_element, col);

            for (IMatrix2D::size_type j = 0; j < max_col; ++j) {
                ACopy(current_row, j) += val * ACopy(row_with_pivot_element, j);
                AInverse(current_row, j) += val * AInverse(row_with_pivot_element, j);
            }

            // Do same transformation on the rhs
            rhs(current_row) += val * rhs(row_with_pivot_element);
        }

        ACopy.print();
        print(ACopy);
    }

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
Gauss::findRowWithPivotElement(Matrix2D const & A, IMatrix2D::size_type column_index) {
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

    IMatrix2D::size_type logical_source_row_index = source_row;//physicalToLogicalRowIndex(source_row);
    IMatrix2D::size_type logical_dest_row_index = physicalToLogicalRowIndex(dest_row);
    IMatrix2D::size_type tmp = partial_pivoting_map_[logical_source_row_index];
     partial_pivoting_map_[logical_source_row_index] = partial_pivoting_map_[logical_dest_row_index];
     partial_pivoting_map_[logical_dest_row_index] = tmp;
//    BOOST_ASSERT_MSG(partial_pivoting_map_[pivot_index] == column_index, "Gauss::adjustPivotingMap: Pivoting error");

    if (partial_pivoting_map_[dest_row] != source_row) {
        int a = 1;
        a++;
    }
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
