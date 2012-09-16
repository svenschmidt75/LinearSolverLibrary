#include "pch.h"

#include "SparseLinearSolver.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


std::tuple<bool, Vector, int>
SparseLinearSolver::sparseSOR(SparseMatrix2D const & m, Vector const & f, double omega, int max_iterations) {
    /* Implements the Successive Over-Relaxation method from
     * "Templates for the Solution of Linear Systems:
     * Building Blocks for Iterative Methods"
     * 
     * Note: For this scheme to converge, matrix m must satisfy
     * the diagonal dominance requirement. Apparently, this is
     * true for all iterative schemes?
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
    double max_l2_norm = 1E-16;
    double l2_norm;

    // Iteration count
    int k = 0;

    do {
        l2_norm = 0;

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
            l2_norm += (correction * correction);

            x(row) += correction;
        }

        // check error term
        l2_norm = sqrt(l2_norm);
    
        k++;

    } while (l2_norm > max_l2_norm && k < max_iterations);

    if (k == max_iterations)
        return std::make_tuple(false, x, k);

    return std::make_tuple(true, x, k);
}
