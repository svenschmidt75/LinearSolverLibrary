#include "pch.h"

#include "SparseLinearSolverUtil.h"
#include "internal/MultiColor.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


bool
SparseLinearSolverUtil::isDiagonallyDominant(SparseMatrix2D const & m) {
    /* Many (if not all) iterative linear solvers require that the
     * matrix m is diagonally dominant for conversion.
     * It is a necessary, but not sufficient condition (?).
     * There are exceptions, i.e. matrices m that are not
     * diagonally dominant and yet the scheme converges.
     */

    // Number of rows
    typedef decltype(m.nelements_.size()) size_type;
    size_type nrows = m.nelements_.size() - 1;

    for (size_type row = 0; row < nrows; ++row) {
        double a_ii = 0;
        bool has_diagonal_element = false;

        // Number of non-zero columns for this row
        size_type ncol = m.nelements_[row + 1] - m.nelements_[row];
        size_type offset = m.nelements_[row];

        double sum = 0;

        for (int icol = 0; icol < ncol; ++icol) {
            if (row == m.columns_[offset + icol]) {
                a_ii = m.elements_[offset + icol];
                has_diagonal_element = true;
                continue;
            }

            // add the total value of the off-diagonal elements in row 'row'
            double a_ij = m.elements_[offset + icol];
            sum += std::fabs(a_ij);
        }

        /* diagonally dominant is a per-row property */
        if (!has_diagonal_element || sum > std::fabs(a_ii))
            return false;
    }

    return true;
}

bool
SparseLinearSolverUtil::isVectorEqual(Vector const & v1, Vector const & v2, double tol = 1E-10) {
    if (v1.size() != v2.size())
        throw std::runtime_error("SparseLinearSolverUtil::isVectorEqual: Vectors have unequal size");

    auto ret = std::mismatch(v1.cbegin(), v1.cend(), v2.cbegin(), [tol](double d1, double d2){
        double delta = std::fabs(d1 - d2);
        return delta < tol;
    });

    return ret.first == v1.cend() && ret.second == v2.cend();
}


SparseMatrix2D
SparseLinearSolverUtil::multicolorDecomposition(SparseMatrix2D const & m) {
     return internal_NS::MultiColor::apply(m);
}
