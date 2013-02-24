/*
 * Name  : ConjugateGradientMethods
 * Path  : 
 * Use   : Implements various conjugate gradient methods
 * Author: Sven Schmidt
 * Date  : 11/08/2012
 */
#pragma once

#include "LinAlg/entities.h"
#include "LinAlg/VectorMath.h"

#include <tuple>

using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

    class ConjugateGradientMethods {
    public:
        template<typename PRECOND>
        static std::tuple<bool, Vector, int> conjugateGradient(SparseMatrix2D const & m, PRECOND const & precond, Vector const & b, int max_iterations = 10000) {
            /* Preconditioned conjugate gradient algorithm
             * as presented in 
             * "Templates for the Solution of Linear Systems: Building Blocks for Iterative Methods",
             * page 13.
             */
            Vector p {b.size()};
            Vector q {b.size()};
            Vector x {b.size()};
            double tol = 1E-10;
            double alpha;

            double normb = VectorMath::norm(b);

            // guessed x = null vector, so initial search direction d = residual r = b
            Vector d = b;
            Vector r { d };

            double residual = VectorMath::norm(d) / normb;
            if (residual <= tol)
                return std::make_tuple(true, b, 0);

            for (int i = 0; i < max_iterations; ++i) {
                Vector Ad = m * d;
                double dAd = VectorMath::dotProduct(d, Ad);
                alpha = VectorMath::dotProduct(r, r) / dAd;

                // improved x_{i+1}
                x += alpha * d;

                // new residual r_{i+1}
                Vector r2 = r - alpha * Ad;

                residual = VectorMath::norm(r2) / normb;
                if (residual <= tol)
                    return std::make_tuple(true, x, i);

                double beta = VectorMath::dotProduct(r2, r2) / VectorMath::dotProduct(r, r);

                // new search direction d_{i + 1}
                d = r2 + beta * d;
            }

#if 0

            Real resid;
            Vector p, z, q;
            Vector alpha(1), beta(1), rho(1), rho_1(1);

            Real normb = norm(b);
            Vector r = b - A*x;

            if (normb == 0.0) 
                normb = 1;

            if ((resid = norm(r) / normb) <= tol) {
                tol = resid;
                max_iter = 0;
                return 0;
            }

            for (int i = 1; i <= max_iter; i++) {
                z = M.solve(r);
                rho(0) = dot(r, z);

                if (i == 1)
                    p = z;
                else {
                    beta(0) = rho(0) / rho_1(0);
                    p = z + beta(0) * p;
                }

                q = A*p;
                alpha(0) = rho(0) / dot(p, q);

                x += alpha(0) * p;
                r -= alpha(0) * q;

                if ((resid = norm(r) / normb) <= tol) {
                    tol = resid;
                    max_iter = i;
                    return 0;     
                }

                rho_1(0) = rho(0);
            }

            tol = resid;
            return 1;
#endif
            // scheme did not converge
            return std::make_tuple(false, b, 10000);
        }

    };

} // LinearSolverLibrary_NS
