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
            Vector x { b.size() };

            double tol = 1E-15;
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
                double dAd = VectorMath::dotProduct(Ad, d);
                alpha = VectorMath::dotProduct(r, r) / dAd;

                // improved x_{i+1}
                x = x + alpha * d;

                // new residual r_{i+1}
                Vector r2 = r - alpha * Ad;

                residual = VectorMath::norm(r2) / normb;
                if (residual <= tol)
                    return std::make_tuple(true, x, i);

                double beta = VectorMath::dotProduct(r2, r2) / VectorMath::dotProduct(r, r);

                r = r2;

                // new search direction d_{i + 1}
                Vector tmp = d;
                d = r + beta * tmp;
            }

            // scheme did not converge
            return std::make_tuple(false, x, 10000);
        }

    };

} // LinearSolverLibrary_NS
