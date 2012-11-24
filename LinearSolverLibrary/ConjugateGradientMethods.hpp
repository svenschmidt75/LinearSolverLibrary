/*
 * Name  : ConjugateGradientMethods
 * Path  : 
 * Use   : Implements various conjugate gradient methods
 * Author: Sven Schmidt
 * Date  : 11/08/2012
 */
#pragma once

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/VectorMath.h"

#include <tuple>

using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

    class ConjugateGradientMethods {
    public:
        template<typename PRECOND>
        static std::tuple<bool, Vector, int> conjugateGradient(SparseMatrix2D const & m, PRECOND const & precond, Vector const & f, int max_iterations = 10000) {
            /* Preconditioned conjugate gradient algorithm
             * as presented in 
             * "Templates for the Solution of Linear Systems: Building Blocks for Iterative Methods",
             * page 13.
             */
            Vector p {f.size()};
            Vector q {f.size()};
            Vector x {f.size()};
            double tol = 1E-10;
            double alpha;

            double normb = VectorMath::norm(f);

            // x = null vector, so r = f
            Vector d = f - m * x;
            Vector r { d };

            double residual = VectorMath::norm(d) / normb;
            if (residual <= tol)
                return std::make_tuple(true, f, 0);

            do {
                Vector Ad = m * d;
                double dAd = VectorMath::dotProduct(d, Ad);
                alpha = VectorMath::dotProduct(r, r) / dAd;

                x += alpha * d;
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

            return std::make_tuple(true, f, 10000);
        }

    };

} // LinearSolverLibrary_NS
