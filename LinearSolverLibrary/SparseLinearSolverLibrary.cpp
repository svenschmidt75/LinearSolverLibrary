#include "pch.h"

#include "SparseLinearSolverLibrary.h"
#include "MatrixDecomposition.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


std::tuple<bool, Vector, int>
SparseLinearSolverLibrary::SparseSOR(SparseMatrix2D const & m, Vector const & f, double omega, int max_iterations) {
    /* Implements the Successive Over-Relaxation method from
     * "Templates for the Solution of Linear Systems:
     * Building Blocks for Iterative Methods"
     * 
     * Note: For this scheme to converge, matrix m must satisfy
     * the diagonal dominance requirement.
     * We use the L_{\infinity} norm.
     *
     * Returns:
     * bool  : indicates whether the iteration succeeded
     * Vector: the solution vector in case of success
     * int   : number of iterations
     */

    common_NS::reporting::checkConditional(m.cols() == f.size(), "AMGSolverSparseLinearSolverLibrary::SparseSOR: Matrix and r.h.s. vector dimensions incompatible");

    using size_type = decltype(m.columns_offset_.size());

    // Number of rows
    size_type nrows = m.columns_offset_.size() - 1;

    // solution vector
    Vector x(f.size());

    // maximum allowed error
    double max_linfinity_norm = 1E-16;
    double l_infinity_norm;
//    double old_l_infinity_norm = 1.0;

    // Iteration count
    int iteration = 0;

    do {
        l_infinity_norm = 0;

        // all rows
        for (size_type row = 0; row < nrows; ++row) {
            double a_ii = 0;

            // number of non-zero columns for this row
            size_type ncol = m.columns_offset_[row + 1] - m.columns_offset_[row];
            size_type offset = m.columns_offset_[row];

            double sigma = 0;

            // the elements column <= row-1 have already been computed
            for (size_type icol = 0; icol < ncol; ++icol) {
                size_type column = m.columns_[offset + icol];
                double a_ij = m.elements_[offset + icol];

                if (row == column) {
                    a_ii = a_ij;
                    continue;
                }

                // sigma = sigma + a(i,j) x(j)^{iteration}
                sigma += a_ij * x(column);
            }

            if (!a_ii)
                throw std::runtime_error("SparseLinearSolver::SparseSOR: Matrix singular. Maybe too few independent equations?");

            sigma = (f(row) - sigma) / a_ii;

            double correction = omega * (sigma - x(row));

            l_infinity_norm = std::max(l_infinity_norm, correction);

            x(row) += correction;
        }
    
        iteration++;

        // check for stalling iteration
//         if (!(iteration % 15)) {
//             double tmp = std::fabs(std::log10(l_infinity_norm / old_l_infinity_norm));
//             tmp = tmp;
//             if (std::fabs(std::log10(l_infinity_norm / old_l_infinity_norm)) < 0.00135)
//                 return std::make_tuple(true, x, iteration);
//             old_l_infinity_norm = l_infinity_norm;
//         }

    } while (l_infinity_norm > max_linfinity_norm && iteration < max_iterations);

    if (iteration == max_iterations)
        return std::make_tuple(false, x, iteration);

    return std::make_tuple(true, x, iteration);
}

std::tuple<bool, Vector, int>
SparseLinearSolverLibrary::SparseSORMultiColor(SparseMatrix2D const & m, Vector const & f, MatrixDecomposition const & mc, double omega, int max_iterations) {
    /* Implements the Successive Over-Relaxation method for
     * multicolor decomposition.
     * 
     * Note: For this scheme to converge, matrix m must satisfy
     * the diagonal dominance requirement.
     * We use the L_{\infinity} norm.
     *

     * Returns:
     * bool  : indicates whether the iteration succeeded
     * Vector: the solution vector in case of success
     * int   : number of iterations
     */

    common_NS::reporting::checkConditional(m.cols() == f.size(), "AMGSolverSparseLinearSolverLibrary::SparseSORMultiColor: Matrix and r.h.s. vector dimensions incompatible");

    using size_type = decltype(m.columns_offset_.size());

    // solution vector
    Vector x(f.size());

    // maximum allowed error
    double max_linfinity_norm = 1E-16;
    double l_infinity_norm;

    // Iteration count
    int iteration = 0;

    do {
        l_infinity_norm = 0;

        /* Compute the x_i for each color. c(x_i) is its color and
         * assume that x_i depends on x_j. Then, if
         * 1. c(x_i) < c(x_j) => take x_j from the previous iteration
         *    iteration - 1, or from the initial condition if iteration = 0
         * 2. c(x_i) > c(x_j) => take x_j from the same iteration iteration as
         *    x_j has already been updated.
         */
        for (auto & variable_set : mc) {

            MatrixDecomposition::color_t color = variable_set.first;
            (void*) color;

            // variable_set is a collection of indices x_i for this color
            for (auto & row : variable_set.second) {
                double a_ii = 0;

                // number of non-zero columns for this row
                size_type ncol = m.columns_offset_[row + 1] - m.columns_offset_[row];
                size_type offset = m.columns_offset_[row];

                double sigma = 0;

                // the elements column <= row-1 have already been computed
                for (size_type icol = 0; icol < ncol; ++icol) {
                    size_type column = m.columns_[offset + icol];
                    double a_ij = m.elements_[offset + icol];

                    if (row == column) {
                        a_ii = a_ij;
                        continue;
                    }

                    // sigma = sigma + a(i,j) x(j)^{iteration}
                    sigma += a_ij * x(column);
                }

                if (!a_ii)
                    throw std::runtime_error("SparseLinearSolver::SparseSORMultiColor: Matrix singular. Maybe too few independent equations?");

                sigma = (f(row) - sigma) / a_ii;

                double correction = omega * (sigma - x(row));

                l_infinity_norm = std::max(l_infinity_norm, correction);

                x(row) += correction;
            }
        }

        // all rows
        iteration++;

    } while (l_infinity_norm > max_linfinity_norm && iteration < max_iterations);

    if (iteration == max_iterations)
        return std::make_tuple(false, x, iteration);

    return std::make_tuple(true, x, iteration);
}

std::tuple<bool, Vector, int>
SparseLinearSolverLibrary::SparseGSMultiColor(SparseMatrix2D const & m, Vector const & x_initial, Vector const & f, MatrixDecomposition const & mc, int max_iterations) {
    /* Implements the Gauss-Seidel method with
     * multicolor decomposition.
     * 
     * Note: For this scheme to converge, matrix m must satisfy
     * the diagonal dominance requirement.
     * We use the L_{\infinity} norm.
     *

     * Returns:
     * bool  : indicates whether the iteration succeeded
     * Vector: the solution vector in case of success
     * int   : number of iterations
     */

    common_NS::reporting::checkConditional(m.cols() == x_initial.size(), "AMGSolverSparseLinearSolverLibrary::SparseGSMultiColor: Matrix and initial vector dimensions incompatible");
    common_NS::reporting::checkConditional(m.cols() == f.size(), "AMGSolverSparseLinearSolverLibrary::SparseGSMultiColor: Matrix and r.h.s. vector dimensions incompatible");

    using size_type = decltype(m.columns_offset_.size());

    // solution vector
    Vector x{x_initial};

    // maximum allowed error
    double max_linfinity_norm = 1E-16;
    double l_infinity_norm;

    // Iteration count
    int iteration = 0;

    do {
        l_infinity_norm = 0;

        /* Compute the x_i for each color. c(x_i) is its color and
         * assume that x_i depends on x_j. Then, if
         * 1. c(x_i) < c(x_j) => take x_j from the previous iteration
         *    iteration - 1, or from the initial condition if iteration = 0
         * 2. c(x_i) > c(x_j) => take x_j from the same iteration iteration as
         *    x_j has already been updated.
         */
        for (auto & variable_set : mc) {

            MatrixDecomposition::color_t color = variable_set.first;
            (void*) color;

            // variable_set is a collection of indices x_i for this color
            for (auto & row : variable_set.second) {
                double a_ii{0};

                // number of non-zero columns for this row
                size_type ncol{m.columns_offset_[row + 1] - m.columns_offset_[row]};
                size_type offset{m.columns_offset_[row]};

                double sigma = 0;

                // the elements column <= row-1 have already been computed
                for (size_type icol{0}; icol < ncol; ++icol) {
                    size_type column{m.columns_[offset + icol]};
                    double a_ij{m.elements_[offset + icol]};

                    if (row == column) {
                        a_ii = a_ij;
                        continue;
                    }

                    // sigma = sigma + a(i,j) x(j)^{iteration}
                    sigma += a_ij * x(column);
                }

                if (!a_ii)
                    throw std::runtime_error("SparseLinearSolver::SparseGSMultiColor: Matrix singular. Maybe too few independent equations?");

                sigma = (f(row) - sigma) / a_ii;
                l_infinity_norm = std::max(l_infinity_norm, sigma);
                x(row) = sigma;
            }
        }

        iteration++;

    } while (l_infinity_norm > max_linfinity_norm && iteration < max_iterations);

    if (iteration == max_iterations)
        return std::make_tuple(false, x, iteration);

    return std::make_tuple(true, x, iteration);
}
