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

namespace {

    void printLanczosVectorsOrthogonal(std::vector<Vector> const & q, IMatrix2D::size_type size) {
        /* The best we can hope for in orthogonality of the Lanczos vectors
         * is <q_i, q_j> = eps if they are orthogonal.
         * Here, we compute the exponent of how much the orthogonality deviates
         * from eps.
         * Example: <q_i, q_j> = 10^{-12}
         * Result: 4, i.e. -12 + 16 (from eps) = 4
         */
        double const machine_eps = std::numeric_limits<double>::epsilon();
        std::cout << std::endl;
        for (IMatrix2D::size_type i = 0; i < size - 1; ++i) {
            double angle = VectorMath::dotProduct(q[i], q[size - 1]);
            double deviation = std::fabs(angle / machine_eps);
            deviation = std::log10(deviation);
            int int_deviation = static_cast<int>(boost::math::round(deviation));
            std::cout << int_deviation << " ";
        }
    }
}

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
    BOOST_ASSERT_MSG(dim == b.size(), "MINRES: Size mismatch");

    setup(dim, normr);
    iteration1(A);
    iteration2(A);

    printLanczosVectorsOrthogonal(q, 3);

    // compute until convergence
    for (SparseMatrix2D::size_type k = 2; k < maxIterations; ++k) {
        k_current = (k_current + 1) % 4;
        k_next = (k_current + 1) % 4;
        k_prev_1 = (k_current - 1 + 4) % 4;
        k_prev_2 = (k_prev_1 - 1 + 4) % 4;

        T[k_current] = 0.0;
        T[k_next] = 0.0;
        T[k_prev_1] = beta;
        T[k_prev_2] = 0.0;

        // reinitialize the r.h.s. vector
        s(k_prev_1) = 0.0;
        s(k_prev_2) = 0.0;
        s(k_next) = 0.0;


        // compute the next basis vector in the iterative QR factorization
        // of A
        w = A * q[current_lanczos_vector_index - 1];

        // TODO: a(i)
        T[k_current] = VectorMath::dotProduct(w, q[current_lanczos_vector_index - 1]);

        w -= T[k_current] * q[current_lanczos_vector_index - 1];

        // TODO: beta = b(i-1)
        w -= beta * q[current_lanczos_vector_index - 2];

        // TODO: beta = b(i)
        normw = beta = VectorMath::norm(w);
        T[k_next] = normw;

        // next normalized basis vector of Krylov space
        q[current_lanczos_vector_index] = w * (1.0 / normw);

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
        p[k_current] = (q[current_lanczos_vector_index - 1] - T[k_prev_1] * p[k_prev_1] - T[k_prev_2] * p[k_prev_2]) * (1.0 / T[k_current]);

        // new approximate solution
        x += s(k_current) * p[k_current];

//         r = b - A * x;
//         normr = VectorMath::norm(r);
//         residual = normr / normb;

        residual = std::fabs(s(k_next) / normb);
        if (residual < tolerance)
            return std::make_tuple(true, x, k, residual);

        current_lanczos_vector_index++;

        printLanczosVectorsOrthogonal(q, current_lanczos_vector_index);
    }

    // scheme did not converge
    return std::make_tuple(false, x, 10000, 0);
}

void
MinresLanPro::setup(LinAlg_NS::SparseMatrix2D::size_type dim, double normr) const {
    // Space for the orthogonal Lanczos vectors.
    // Due to A being symmetric, we only need to remember 3 of them,
    // making use of the three-term recurrence formula.
    q.resize(dim + 1, Vector(dim));

    a.resize(dim + 1, Vector(dim));
    b.resize(dim + 1, Vector(dim));

    // search directions
    p.resize(3 + 1, Vector(dim));

    // approximate solution vector
    x = Vector(dim);
    w = Vector(dim);

    // TODO: b(0)
    beta = 0.0;

    // initialize the Lanczos iteration
    k_current = 2;
    k_next = 3;
    k_prev_1 = 1;
    k_prev_2 = 0;
    current_lanczos_vector_index = 1;

    // initialize r.h.s. vector
    s(k_current) = normr;

    q[current_lanczos_vector_index - 1] = r * (1.0 / normr);
}

void
MinresLanPro::iteration1(SparseMatrix2D const & A) const {
    /***********************
     * Lanczos iteration 1 *
     ***********************/

    // compute the 1st basis vector in the iterative QR factorization
    // of A
    w = A * q[current_lanczos_vector_index - 1];

    // TODO: a(i)
    T[k_current] = VectorMath::dotProduct(w, q[current_lanczos_vector_index - 1]);

    w -= T[k_current] * q[current_lanczos_vector_index - 1];

    double normw = beta = VectorMath::norm(w);
    T[k_next] = normw;

    // next normalized basis vector of Krylov space
    q[current_lanczos_vector_index] = w * (1.0 / normw);

    // compute the Givens rotation that annihilates T[k_next]
    ResHelper::GeneratePlaneRotation(T[k_current], T[k_next], cs(k_current), sn(k_current));

    // apply Givens rotation to r.h.s. vector s
    ResHelper::ApplyPlaneRotation(s(k_current), s(k_next), cs(k_current), sn(k_current));

    // using the Givens coefficients cn and sn, eliminate
    // T[k_next] to make it upper triangular
    ResHelper::ApplyPlaneRotation(T[k_current], T[k_next], cs(k_current), sn(k_current));

    // compute search vector
    p[k_current] = q[current_lanczos_vector_index - 1] * (1.0 / T[k_current]);

    // new approximate solution
    x += s(k_current) * p[k_current];

    current_lanczos_vector_index++;
}

void
MinresLanPro::iteration2(SparseMatrix2D const & A) const {
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
    T[k_current] = 0.0;
    T[k_next] = 0.0;

    // initialize column with b(2) above
    T[k_prev_1] = beta;
    T[k_prev_2] = 0.0;

    // reinitialize the r.h.s. vector
    s(k_prev_1) = 0.0;
    s(k_prev_2) = 0.0;
    s(k_next) = 0.0;


    // compute the 2nd basis vector in the iterative QR factorization
    // of A
    w = A * q[current_lanczos_vector_index - 1];

    T[k_current] = VectorMath::dotProduct(w, q[current_lanczos_vector_index - 1]);

    w -= T[k_current] * q[current_lanczos_vector_index - 1];
    w -= beta * q[current_lanczos_vector_index - 2];

    normw = beta = VectorMath::norm(w);
    T[k_next] = normw;

    // next normalized basis vector of Krylov space
    q[current_lanczos_vector_index] = w * (1.0 / normw);

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
    p[k_current] = (q[current_lanczos_vector_index - 1] - T[k_prev_1] * p[k_prev_1]) * (1.0 / T[k_current]);

    // new approximate solution
    x += s(k_current) * p[k_current];

    current_lanczos_vector_index++;
}

MinresLanPro::Return_t
MinresLanPro::solve(SparseMatrix2D const & A, Vector const & b, int maxIterations, double tolerance = 1E-15) {
    MinresLanPro minres;
    return minres.solve_internal(A, b, maxIterations, tolerance);
}

} // LinearSolverLibrary_NS
