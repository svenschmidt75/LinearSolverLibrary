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

    /* Return type of iterative CG solvers.
     * bool: success or failure
     * Vector: solution x
     * int: Number of iterations needed
     * double: Tolerance achieved
     */
    typedef std::tuple<bool, Vector, int, double> Return_t;

    class ConjugateGradientMethods {
    public:
        template<typename PRECOND>
        static Return_t CG(SparseMatrix2D const & m, PRECOND const & precond, Vector const & b, int max_iterations = 10000) {
            /* Preconditioned conjugate gradient algorithm
             * as presented in 
             * "Templates for the Solution of Linear Systems: Building Blocks for Iterative Methods",
             * page 13.
             */
            Vector x(b.size());

            double tol = 1E-15;
            double alpha;

            double normb = VectorMath::norm(b);

            // guessed x = null vector, so initial search direction d = residual r = b
            Vector d = b;
            Vector r(d);

            double residual = VectorMath::norm(d) / normb;
            if (residual <= tol)
                return std::make_tuple(true, b, 0, residual);

            for (int i = 0; i < max_iterations; ++i) {
                // most expensive operation
                Vector Ad = m * d;
                double dAd = VectorMath::dotProduct(Ad, d);
                double rr = VectorMath::dotProduct(r, r);
                alpha = rr / dAd;

                // improved x_{i+1}
                x += alpha * d;

                // new residual r_{i+1}
                Vector r2 = r - alpha * Ad;

                residual = VectorMath::norm(r2) / normb;
                if (residual <= tol)
                    return std::make_tuple(true, x, i, residual);

//                std::cout << residual << std::endl;

                double beta = VectorMath::dotProduct(r2, r2) / rr;

                r = r2;

                // new search direction d_{i + 1}
                d = r + beta * d;
            }

            // scheme did not converge
            return std::make_tuple(false, x, 10000, 0);
        }

        template<typename PRECOND>
        static Return_t BiCG(SparseMatrix2D const & m, SparseMatrix2D const & m_transposed, PRECOND const & precond, Vector const & b, int max_iterations = 10000) {
            /* Preconditioned biconjugate conjugate gradient algorithm
             * as presented in 
             * "Templates for the Solution of Linear Systems: Building Blocks for Iterative Methods",
             * page 20.
             */
            Vector x(b.size());

            double tol = 1E-15;
            double alpha;

            double normb = VectorMath::norm(b);

            // guessed x = null vector
            Vector r1(b);

            double residual = VectorMath::norm(r1) / normb;
            if (residual <= tol)
                return std::make_tuple(true, b, 0, residual);

            Vector r2(b);
            Vector p1(r1);
            Vector p2(r2);
            double rho;
            double rho_prev;

            for (int i = 0; i < max_iterations; ++i) {
                rho = VectorMath::dotProduct(r1, r2);
                if (!rho) {
                    // r1 and r2 orthogonal
                    double residual = VectorMath::norm(r1) / normb;
                    return std::make_tuple(false, x, i, residual);
                }

                if (i > 1) {
                    beta = rho / rho_prev;
                    p1 = r1 + beta * p1;
                    p2 = r2 + beta * p2;
                }

                // most expensive operation
                Vector q1 = m * p1;
                Vector q2 = m_transposed * p2;

                alpha = rho / VectorMath::dotProduct(p2, q1);
                x += alpha * p1;
                r1 -= alpha * q1;
                r2 -= alpha * q2;

                rho_prev = rho;

                residual = VectorMath::norm(r1) / normb;
                if (residual <= tol)
                    return std::make_tuple(true, x, i, residual);
            }

            // scheme did not converge
            return std::make_tuple(false, x, 10000, 0);
        }


#if 0
        z = M.solve(r);
        ztilde = M.trans_solve(rtilde);
        rho_1(0) = dot(z, rtilde);
        if (rho_1(0) == 0) { 
            tol = norm(r) / normb;
            max_iter = i;
            return 2;
        }
        if (i == 1) {
            p = z;
            ptilde = ztilde;
        } else {
            beta(0) = rho_1(0) / rho_2(0);
            p = z + beta(0) * p;
            ptilde = ztilde + beta(0) * ptilde;
        }
        q = A * p;
        qtilde = A.trans_mult(ptilde);
        alpha(0) = rho_1(0) / dot(ptilde, q);
        x += alpha(0) * p;
        r -= alpha(0) * q;
        rtilde -= alpha(0) * qtilde;

        rho_2(0) = rho_1(0);
        if ((resid = norm(r) / normb) < tol) {
            tol = resid;
            max_iter = i;
            return 0;
        }
  }
#endif


    };

} // LinearSolverLibrary_NS
