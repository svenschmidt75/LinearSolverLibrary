#include "pch.h"

#include "SparseLinearSolverLibrary.h"
#include "MatrixDecomposition.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


std::tuple<bool, Vector, int>
SparseLinearSolverLibrary::sparseSOR(SparseMatrix2D const & m, Vector const & f, double omega, int max_iterations) {
    /* Implements the Successive Over-Relaxation method from
     * "Templates for the Solution of Linear Systems:
     * Building Blocks for Iterative Methods"
     * 
     * Note: For this scheme to converge, matrix m must satisfy
     * the diagonal dominance requirement. Apparently, this is
     * true for all iterative schemes?
     * We use the Linfinity norm.
     *
     * Returns:
     * bool  : indicates whether the iteration succeeded
     * Vector: the solution vector in case of success
     * int   : number of iterations
     */

    // Number of rows
    typedef decltype(m.nelements_.size()) size_type;
    size_type nrows = m.nelements_.size() - 1;

    // solution vector
    Vector x(f.size());

    // maximum allowed error
    double max_linfinity_norm = 1E-16;
    double l_infinity_norm;

    // Iteration count
    int k = 0;

    do {
        l_infinity_norm = 0;

        // all rows
        for (size_type row = 0; row < nrows; ++row) {
            double a_ii = 0;

            // number of non-zero columns for this row
            size_type ncol = m.nelements_[row + 1] - m.nelements_[row];
            size_type offset = m.nelements_[row];

            double sigma = 0;

            // the elements col <= row-1 have already been computed
            for (size_type icol = 0; icol < ncol; ++icol) {
                size_type col = m.columns_[offset + icol];
                double a_ij = m.elements_[offset + icol];

                if (row == col) {
                    a_ii = a_ij;
                    continue;
                }

                // sigma = sigma + a(i,j) x(j)^{k}
                sigma += a_ij * x(col);
            }

            if (!a_ii)
                throw std::runtime_error("SparseLinearSolver::sparseSOR: Matrix singular. Maybe too few independent equations?");

            sigma = (f(row) - sigma) / a_ii;

            double correction = omega * (sigma - x(row));

            l_infinity_norm = std::max(l_infinity_norm, correction);

            x(row) += correction;
        }
    
        k++;

    } while (l_infinity_norm > max_linfinity_norm && k < max_iterations);

    if (k == max_iterations)
        return std::make_tuple(false, x, k);

    return std::make_tuple(true, x, k);
}

std::tuple<bool, Vector, int>
SparseLinearSolverLibrary::sparseSORMultiColor(SparseMatrix2D const & m, Vector const & f, MatrixDecomposition const & mc, double omega, int max_iterations) {
    /* Implements the Successive Over-Relaxation method for
     * multicolor decomposition.
     * 
     * Note: For this scheme to converge, matrix m must satisfy
     * the diagonal dominance requirement. Apparently, this is
     * true for all iterative schemes?
     * We use the Linfinity norm.
     *

     * Returns:
     * bool  : indicates whether the iteration succeeded
     * Vector: the solution vector in case of success
     * int   : number of iterations
     */

    // Number of rows
    typedef decltype(m.nelements_.size()) size_type;

    // solution vector
    Vector x(f.size());

    // maximum allowed error
    double max_linfinity_norm = 1E-16;
    double l_infinity_norm;

    // Iteration count
    int k = 0;

    do {
        l_infinity_norm = 0;

        /* Compute the x_i for each color. c(x_i) is its color and
         * assume that x_i depends on x_j. Then, if
         * 1. c(x_i) < c(x_j) => take x_j from the previous iteration
         *    k - 1, or from the initial condition if k = 0
         * 2. c(x_i) > c(x_j) => take x_j from the same iteration k as
         *    x_j has already been updated.
         */
        for (auto & indep_set : mc) {

            MatrixDecomposition::color_t color = indep_set.first;
            (void*) color;

            // indep_set is a collection of indices x_i for this color
            for (auto & row : indep_set.second) {
                double a_ii = 0;

                // number of non-zero columns for this row
                size_type ncol = m.nelements_[row + 1] - m.nelements_[row];
                size_type offset = m.nelements_[row];

                double sigma = 0;

                // the elements col <= row-1 have already been computed
                for (size_type icol = 0; icol < ncol; ++icol) {
                    size_type col = m.columns_[offset + icol];
                    double a_ij = m.elements_[offset + icol];

                    if (row == col) {
                        a_ii = a_ij;
                        continue;
                    }

                    // sigma = sigma + a(i,j) x(j)^{k}
                    sigma += a_ij * x(col);
                }

                if (!a_ii)
                    throw std::runtime_error("SparseLinearSolver::sparseSORMultiColor: Matrix singular. Maybe too few independent equations?");

                sigma = (f(row) - sigma) / a_ii;

                double correction = omega * (sigma - x(row));

                l_infinity_norm = std::max(l_infinity_norm, correction);

                x(row) += correction;
            }
        }

        // all rows
        k++;

    } while (l_infinity_norm > max_linfinity_norm && k < max_iterations);

    if (k == max_iterations)
        return std::make_tuple(false, x, k);

    return std::make_tuple(true, x, k);
}