#include "pch.h"

#include "SparseLinearSolver.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;

#if 0
std::tuple<bool, Vector>
SparseLinearSolver::sparseSOR(SparseMatrix2D const & A, Vector const & f, double omega) {
    /* Implements the Successive OverReleaxation method from
     * "Templates for the Solution of Linear Systems:
     * Building Blocks for Iterative Methods"
     * 
     * Note: For this scheme to converge, matrix A must satisfy
     * the diagonal dominance requirement. Apparently, this is
     * true for all iterative schemes?
     */
    bool check_diagonally_dominant = false;

    // Number of rows
    typedef decltype(A.nelements_.size()) size_type;
    size_type nrows = A.nelements_.size() - 1;

    // check if the system is diagonally dominant
    if (check_diagonally_dominant) {
        for (int row = 0; row < nrows; ++row) {
            double a_ii = 0;
            bool has_diagonal_element = false;

            // Number of non-zero columns for this row
            boost::uint64_t ncol = A.nelements_[row + 1] - A.nelements_[row];
            boost::uint64_t offset = A.nelements_[row];

            double sum = 0;

            for (int icol = 0; icol < ncol; ++icol) {
                if (row == A.columns_[offset + icol]) {
                    a_ii = A.elements_[offset + icol];
                    has_diagonal_element = true;
                    continue;
                }

                // add the total value of the off-diagonal elements in row i
                double a_ij = A.elements_[offset + icol];
                sum += a_ij;
            }

            if (!has_diagonal_element || sum > a_ii)
                throw std::exception("LinearSolver::SOR: Matrix not diagonally dominant");
        }
    }

    IMatrix2D::Vec x(f.size(), 0);

    // Maximum allowed error
    double max_l2_norm = 1E-16;
    double l2_norm;

    // Iteration count
    int k = 0;

    do {
        l2_norm = 0;

        // All rows
        for (int row = 0; row < nrows; ++row) {
            double a_ii = 0;

            // Number of non-zero columns for this row
            boost::uint64_t ncol = A.nelements_[row + 1] - A.nelements_[row];
            boost::uint64_t offset = A.nelements_[row];

            double sigma = 0;

            // The elements col <= row-1 have already been computed
            for (int icol = 0; icol < ncol; ++icol) {
                boost::uint64_t col = A.columns_[offset + icol];
                double a_ij = A.elements_[offset + icol];

                if (row == col) {
                    a_ii = a_ij;
                    continue;
                }

                // sigma = sigma + a(i,j) x(j)^{k}
                sigma += a_ij * x[col];
            }

            if (!a_ii)
                throw std::exception("LinearSolver::SOR: Matrix singular. Maybe too few independent equations?");

            sigma = (f[row] - sigma) / a_ii;

            double correction = omega * (sigma - x[row]);
            l2_norm += (correction * correction);

            x[row] += correction;
        }

        // Check error term
        l2_norm /= (nrows + 1);
        l2_norm = sqrt(l2_norm);
    
        k++;

    } while (l2_norm > max_l2_norm && k < 10000);

    if (k == 10000)
        return std::make_tuple(false, x);

    return std::make_tuple(true, x);
}
#endif
