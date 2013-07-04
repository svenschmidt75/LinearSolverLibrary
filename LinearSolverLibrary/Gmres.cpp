#include "pch.h"

#include "Gmres.h"
#include "ResHelper.h"
#include "LinAlg/VectorMath.h"
#include "LinAlg/entities.h"
#include "LinAlg/operators.h"
#include "LinAlg/UHMatrix.h"


using namespace LinAlg_NS;


namespace {

template<typename MATRIX>
static void Update(Vector & x, typename MATRIX::size_type k, MATRIX const & H, Vector const & s, std::vector<Vector> const & q) {
    /* H(k + 1) (i.e. i = 0, ..., k) is upper triangular.
     * Solve H(i + 1) * y = s via back-substitution. 
     * Numerical Linear Algebra, Trefethen, Bau, Alg. 17.1, page 122.
     * Then, compute x = x + q[i] * y[i], i = 0, ..., k
     */

    // initialize y with a copy of the r.h.s. s
    Vector y(s);

    // solve via back-substitution
    for (typename MATRIX::size_type i = 0; i <= k; ++i) {
        typename MATRIX::size_type index = k - i;

        y(index) /= H(index, index);

        // y(index) is known at this point.
        // Solve for all H(row, col) where col >= row because
        // H(row, col) = 0 for col < row.
        for (typename MATRIX::size_type j = 0; j < index; ++j) {
            typename MATRIX::size_type row = index - j - 1;
            y(row) -= H(row, index) * y(index);
        }
    }

    // compute approximate new solution
    for (typename MATRIX::size_type j = 0; j <= k; ++j)
        x += q[j] * y(j);
}

}

namespace LinearSolverLibrary_NS {

Gmres::Return_t
Gmres::solve(SparseMatrix2D const & A, Vector const & b, SparseMatrix2D::size_type m, int maxIterations, double tolerance) {
    /* Implements the GMRES(m) restarted algorithm from
     *  Reference:
     *  R. Barrett et al, "Templates for the Solution of Linear Systems:
     *  Building Blocks for Iterative Methods", SIAM, 1994.
     */
    double normb = VectorMath::norm(b);

    // guessed x = null vector
    Vector r(b);

    double beta = VectorMath::norm(r);

    double residual = beta / normb;
    if (residual <= tolerance)
        return std::make_tuple(true, b, 0, residual);

    auto dim = A.cols();

    // space for the orthogonal Arnoldi vectors
    std::vector<Vector> q(m + 1, Vector(dim));

#if 0
    BOOST_SCOPE_EXIT(&q) {
        // check orthogonality of Arnoldi vectors
        std::cout << std::endl;

        for (SparseMatrix2D::size_type k = 1; k < q.size(); ++k) {
            double test = VectorMath::dotProduct(q[k], q[k - 1]);
            std::cout << test << std::endl;
            std::cout << std::endl;
        }
    } BOOST_SCOPE_EXIT_END
#endif

    // the Givens coefficients
    Vector s(m + 1), cs(m + 1), sn(m + 1), w(dim);

    SparseMatrix2D::size_type j = 1;

    UHMatrix H(m + 1);

    Vector x(b.size());

    while (j <= maxIterations) {
        q[0] = r * (1.0 / beta);
        s(0) = beta;

        for (SparseMatrix2D::size_type i = 0; i < m && j <= maxIterations; ++i, ++j) {
            // compute the next basis vector in the iterative QR factorization
            // of A
            w = A * q[i];

            // compute the Hessenberg coefficients
            for (int k = 0; k <= i; ++k) {
                H(k, i) = VectorMath::dotProduct(w, q[k]);
                w -= H(k, i) * q[k];
            }

            double normw = VectorMath::norm(w);
            H(i + 1, i) = normw;

            // next normalized basis vector of Krylov space
            q[i + 1] = w * (1.0 / normw);

//             H.print();

            // apply all previous Givens rotations to the last column
            // of H. See Iterative Methods for Solving Linear systems,
            // Anne Greenbaum, page 40
            for (int k = 0; k < i; ++k)
                // apply Givens rotation to column i, row 0, ..., i - 1
                ResHelper::ApplyPlaneRotation(H(k, i), H(k + 1, i), cs(k), sn(k));

//             H.print();

            // compute the Givens coefficients cs and sn
            ResHelper::GeneratePlaneRotation(H(i, i), H(i + 1, i), cs(i), sn(i));

            // using the Givens coefficients cn and sn, eliminate
            // H(i + 1, i) to make it upper triangular
            ResHelper::ApplyPlaneRotation(H(i, i), H(i + 1, i), cs(i), sn(i));

//             H.print();

            // apply Givens rotation to r.h.s. vector s
            ResHelper::ApplyPlaneRotation(s(i), s(i + 1), cs(i), sn(i));

            // s(k) = 0, except for k = i + 1, which equals the norm
            // of the residual
            residual = std::fabs(s(i + 1) / normb);
            if (residual < tolerance) {
                // solve for x
                Update(x, i, H, s, q);
                return std::make_tuple(true, x, j, residual);
            }
        }

        // solve for x and use as new initial x for restart
        Update(x, m - 1, H, s, q);

        r = b - A * x;
        beta = VectorMath::norm(r);
        residual = beta / normb;
        if (residual < tolerance)
            return std::make_tuple(true, x, j, residual);

        // reset r.h.s. to zero
        s.clear();
    }

    // scheme did not converge
    return std::make_tuple(false, x, 10000, 0);

}

} // LinearSolverLibrary_NS
