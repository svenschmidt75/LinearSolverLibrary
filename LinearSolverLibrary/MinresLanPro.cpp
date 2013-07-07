#include <pch.h>

#include "MinresLanPro.h"
#include "ResHelper.h"
#include "LinAlg/VectorMath.h"
#include "LinAlg/entities.h"
#include "LinAlg/operators.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

MinresLanPro::MinresLanPro() 
    :
    s(3 + 1),
    cs(3 + 1),
    sn(3 + 1),
    w(3 + 1) {}

MinresLanPro::Return_t
MinresLanPro::solve_internal(SparseMatrix2D const & A, Vector const & b, int maxIterations, double tolerance = 1E-15) const {
    /* Implements the MINRES algorithm from
     *  Reference:
     *  Anne Greenbaum, Iterative Methods for Solving Linear Systems,
     *  SIAM 1997
     * Used for symmetric matrices that are not necessarily pos. def.
     */
    double normb = VectorMath::norm(b);

    // initial guess x_{0} = null
    r = Vector(b);
    double normr = VectorMath::norm(r);
    double residual = normr / normb;
    if (residual <= tolerance)
        return std::make_tuple(true, b, 0, residual);

    auto dim = A.cols();
    BOOST_ASSERT_MSG(dim == b.size(), "MinresLanPro: Size mismatch");

    lanczos.init(A, r * (1.0 / normr));

    setup(dim, normr);
    iteration1();
    iteration2();

    // compute until convergence
    for (SparseMatrix2D::size_type k = 2; k < maxIterations; ++k) {
        k_current = (k_current + 1) % 4;
        k_next = (k_current + 1) % 4;
        k_prev_1 = (k_current - 1 + 4) % 4;
        k_prev_2 = (k_prev_1 - 1 + 4) % 4;

        lanczos.computeNextLanczosVector();
        Vector const & q = lanczos.getPreviousLanczosVector();

        T[k_next] = lanczos.getCurrentBeta();
        T[k_current] = lanczos.getCurrentAlpha();
        T[k_prev_1] = lanczos.getPreviousBeta();
        T[k_prev_2] = 0.0;

        // reinitialize the r.h.s. vector
        s(k_prev_1) = 0.0;
        s(k_prev_2) = 0.0;
        s(k_next) = 0.0;

        // apply previous rotation
        ResHelper::ApplyPlaneRotation(T[k_prev_2], T[k_prev_1], cs(k_prev_2), sn(k_prev_2));
        ResHelper::ApplyPlaneRotation(T[k_prev_1], T[k_current], cs(k_prev_1), sn(k_prev_1));

        // compute the Givens rotation that annihilates T[k_next]
        ResHelper::GeneratePlaneRotation(T[k_current], T[k_next], cs(k_current), sn(k_current));

        // apply Givens rotation to r.h.s. vector s
        ResHelper::ApplyPlaneRotation(s(k_current), s(k_next), cs(k_current), sn(k_current));

        // using the Givens coefficients cn and sn, eliminate
        // T[k_next] to make it upper triangular
        ResHelper::ApplyPlaneRotation(T[k_current], T[k_next], cs(k_current), sn(k_current));

        // compute search vector
        p[k_current] = (q - T[k_prev_1] * p[k_prev_1] - T[k_prev_2] * p[k_prev_2]) * (1.0 / T[k_current]);

        // new approximate solution
        x += s(k_current) * p[k_current];

//         r = b - A * x;
//         normr = VectorMath::norm(r);
//         residual = normr / normb;

        residual = std::fabs(s(k_next) / normb);
        if (residual < tolerance)
            return std::make_tuple(true, x, k, residual);
    }

    // scheme did not converge
    return std::make_tuple(false, x, 10000, 0);
}

void
MinresLanPro::setup(LinAlg_NS::SparseMatrix2D::size_type dim, double normr) const {
    // search directions
    p.resize(3 + 1, Vector(dim));

    // approximate solution vector
    x = Vector(dim);
    w = Vector(dim);

    // initialize the Lanczos iteration
    k_current = 2;
    k_next = 3;
    k_prev_1 = 1;
    k_prev_2 = 0;

    // initialize r.h.s. vector
    s(k_current) = normr;
}

void
MinresLanPro::iteration1() const {
    /***********************
     * Lanczos iteration 1 *
     ***********************/
    Vector const & q = lanczos.getPreviousLanczosVector();
    T[k_next] = lanczos.getCurrentBeta();
    T[k_current] = lanczos.getCurrentAlpha();

    // compute the Givens rotation that annihilates T[k_next]
    ResHelper::GeneratePlaneRotation(T[k_current], T[k_next], cs(k_current), sn(k_current));

    // apply Givens rotation to r.h.s. vector s
    ResHelper::ApplyPlaneRotation(s(k_current), s(k_next), cs(k_current), sn(k_current));

    // using the Givens coefficients cn and sn, eliminate
    // T[k_next] to make it upper triangular
    ResHelper::ApplyPlaneRotation(T[k_current], T[k_next], cs(k_current), sn(k_current));

    // compute search vector
    p[k_current] = q * (1.0 / T[k_current]);

    // new approximate solution
    x += s(k_current) * p[k_current];
}

void
MinresLanPro::iteration2() const {
    /***********************
     * Lanczos iteration 2 *
     ***********************/

    k_current = (k_current + 1) % 4;
    k_next = (k_current + 1) % 4;
    k_prev_1 = (k_current - 1 + 4) % 4;
    k_prev_2 = (k_prev_1 - 1 + 4) % 4;

    /* The tridiagonal structure of T is like
     *
     * | a(1) b(1)               |
     * | b(1) a(2) b(2)          |
     * |      b(2) a(3) b(3)     |
     * |        .  .  .          |
     * |         .  .  .         |
     * |          .  .  . b(k-1) |
     * |           b(k-1) a(k)   |
     */
    lanczos.computeNextLanczosVector();
    Vector const & q = lanczos.getPreviousLanczosVector();
    T[k_prev_1] = lanczos.getPreviousBeta();
    T[k_next] = lanczos.getCurrentBeta();
    T[k_current] = lanczos.getCurrentAlpha();
    T[k_prev_2] = 0.0;

    // reinitialize the r.h.s. vector
    s(k_prev_1) = 0.0;
    s(k_prev_2) = 0.0;
    s(k_next) = 0.0;

    // apply previous rotation
    ResHelper::ApplyPlaneRotation(T[k_prev_1], T[k_current], cs(k_prev_1), sn(k_prev_1));

    // compute the Givens rotation that annihilates T[k_next]
    ResHelper::GeneratePlaneRotation(T[k_current], T[k_next], cs(k_current), sn(k_current));

    // apply Givens rotation to r.h.s. vector s
    ResHelper::ApplyPlaneRotation(s(k_current), s(k_next), cs(k_current), sn(k_current));

    // using the Givens coefficients cn and sn, eliminate
    // T[k_next] to make it upper triangular
    ResHelper::ApplyPlaneRotation(T[k_current], T[k_next], cs(k_current), sn(k_current));

    // compute search vector
    p[k_current] = (q - T[k_prev_1] * p[k_prev_1]) * (1.0 / T[k_current]);

    // new approximate solution
    x += s(k_current) * p[k_current];
}

MinresLanPro::Return_t
MinresLanPro::solve(SparseMatrix2D const & A, Vector const & b, int maxIterations, double tolerance = 1E-15) {
    MinresLanPro minres;
    return minres.solve_internal(A, b, maxIterations, tolerance);
}

} // LinearSolverLibrary_NS
