/*
 * Name  : ConjugateGradientMethods
 * Path  : 
 * Use   : Implements various conjugate gradient methods
 * Author: Sven Schmidt
 * Date  : 11/08/2012
 */
#pragma once

#include "GCR.h"
#include "Minres.h"
#include "MinresLanPro.h"
#include "Gmres.h"
#include "LinAlg/BasicEntityOperators.h"
#include "LinAlg/VectorMath.h"
#include "LinAlg/UHMatrix.h"

#include <tuple>

#include <boost/scope_exit.hpp>


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

    /* Return type of iterative CG solvers.
     * bool: success or failure
     * Vector: solution x
     * int: Number of iterations needed
     * double: Tolerance achieved
     */
    typedef std::tuple<bool, Vector, IMatrix2D::size_type, double> Return_t;
    
    class ConjugateGradientMethods {
    public:
        static Return_t CG(SparseMatrix2D const & m, Vector const & b, int max_iterations = 10000) {
            /* Unpreconditioned conjugate gradient algorithm
             * as presented in 
             * "Templates for the Solution of Linear Systems: Building Blocks for Iterative Methods",
             * page 13.
             * Use for when A is Hermitian positive definite, as
             * Hermitian: all eigenvalues are real
             * pos. def.: all eigenvalues >= 0
             * Here, we only consider real A, i.e. Hermitian = symmetric.
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

//                std::cout << "Iteration " << i << ": " << residual << std::endl;

                double beta = VectorMath::dotProduct(r2, r2) / rr;

                r = r2;

                // new search direction d_{i + 1}
                d = r + beta * d;
            }

            // scheme did not converge
            return std::make_tuple(false, x, max_iterations, 0);
        }

        static Return_t BiCG(SparseMatrix2D const & m, SparseMatrix2D const & m_transposed, Vector const & b, int max_iterations = 10000) {
            /* Biconjugate gradient algorithm
             * as presented in 
             * "Templates for the Solution of Linear Systems: Building Blocks for Iterative Methods",
             * page 20.
             * Use BiCG for indefinite A, i.e. eigenvalues are not required >= 0.
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
            double rho_prev = VectorMath::dotProduct(r1, r2);

            for (SparseMatrix2D::size_type i = 0; i < max_iterations; ++i) {
                // most expensive operation
                Vector q1 = m * p1;
                Vector q2 = m_transposed * p2;

                alpha = rho_prev / VectorMath::dotProduct(p1, q2);
                x += alpha * p1;
                r1 -= alpha * q1;

                residual = VectorMath::norm(r1) / normb;
                if (residual <= tol)
                    return std::make_tuple(true, x, i, residual);

//                 std::cout << "Iteration " << i << ": " << residual << std::endl;

                r2 -= alpha * q2;

                rho = VectorMath::dotProduct(r1, r2);
                double beta = rho / rho_prev;
                p1 = r1 + beta * p1;
                p2 = r2 + beta * p2;

                rho_prev = rho;
            }

            // scheme did not converge
            return std::make_tuple(false, x, max_iterations, 0);
        }

        static Return_t BiCGSTAB(SparseMatrix2D const & m, Vector const & b, int max_iterations = 10000) {
            /* Biconjugate gradient stabilized algorithm
             * as presented in 
             * Iterative Methods for Solving Linear Systems
             * Anne Greenbaum, page 91.
             * Similar to BiCG (A indefinite), but without the need for A^{T} and
             * better stability.
             */
            double tol = 1E-15;

            double normb = VectorMath::norm(b);

            // guessed x = null vector
            Vector r(b);

            double residual = VectorMath::norm(r) / normb;
            if (residual <= tol)
                return std::make_tuple(true, b, 0, residual);

            Vector x(b.size());
            Vector x_half(b.size());
            Vector r0(b);
            Vector r_half(b.size());
            Vector p(r);
            double alpha;
            double rho;
            double rho_prev = VectorMath::dotProduct(r, r0);

            for (SparseMatrix2D::size_type i = 0; i < max_iterations; ++i) {
                // most expensive operation
                Vector q = m * p;

                alpha = rho_prev / VectorMath::dotProduct(q, r0);

                x_half = x + alpha * p;
                r_half = r - alpha * q;

                Vector q2 = m * r_half;

                double omega = VectorMath::dotProduct(r_half, q2) / VectorMath::dotProduct(q2, q2);
                x = x_half + omega * r_half;
                r = r_half - omega * q2;

                residual = VectorMath::norm(r) / normb;
                if (residual <= tol)
                    return std::make_tuple(true, x, i, residual);

//                std::cout << "Iteration " << i << ": " << residual << std::endl;

                rho = VectorMath::dotProduct(r, r0);

                double beta = alpha / omega * rho / rho_prev;
                p = r + beta * (p - omega * q);

                rho_prev = rho;
            }

            // scheme did not converge
            return std::make_tuple(false, x, max_iterations, 0);
        }

        static Return_t TFQMR(SparseMatrix2D const & A, Vector const & b, int maxIterations = 10000) {
            /* Transpose-Free Quasi-Minimal Residual method for nonsymmetric linear problems
             * as presented in 
             * Iterative Methods for Sparse Linear Systems, Y. Saad, 2nd edition and
             * Iterative Methods for Linear and Nonlinear Equations, C.T. Kelly.
             */
            double tol = 1E-15;

            auto dim = A.cols();
            BOOST_ASSERT_MSG(dim == b.size(), "TFQMR: Size mismatch");

            Vector w = b;
            Vector y1 = b;
            Vector r0 = b;
            Vector u1 = A * y1;
            Vector v = u1;
            Vector d(dim);
            Vector x(dim);

            double normb = VectorMath::norm(b);
            double tau = normb;
            double rho = tau * tau;
            double theta = 0;
            double eta = 0;
            double residual;

            for (SparseMatrix2D::size_type i = 0; i < maxIterations; ++i) {
                double sigma = VectorMath::dotProduct(r0, v);
                if (sigma == 0)
                    return std::make_tuple(false, x, i, 0);
                double alpha = rho / sigma;

                // odd branch
                w -= alpha * u1;
                d = y1 + (theta * theta * eta / alpha) * d;
                theta = VectorMath::norm(w) / tau;
                double c = 1.0 / std::sqrt(1.0 + theta * theta);
                tau = tau * theta * c;
                eta = c * c * alpha;
                x += eta * d;
                SparseMatrix2D::size_type m = 2 * (i + 1) - 2 + 1;
                if (tau * std::sqrt(m + 1) < tol * normb)
                    return std::make_tuple(true, x, i, tau * std::sqrt(m + 1));

                // even branch
                Vector y2 = y1 - alpha * v;
                Vector u2 = A * y2;
                w -= alpha * u2;
                d = y2 + (theta * theta * eta / alpha) * d;
                theta = VectorMath::norm(w) / tau;
                c = 1.0 / std::sqrt(1.0 + theta * theta);
                tau = tau * theta * c;
                eta = c * c * alpha;
                x += eta * d;
                m += 1;
                if (tau * std::sqrt(m + 1) < tol * normb)
                    return std::make_tuple(true, x, i, tau * std::sqrt(m + 1));

                if (rho == 0)
                    return std::make_tuple(false, x, i, 0);

                double rho2 = VectorMath::dotProduct(r0, w);
                double beta = rho2 / rho;
                rho = rho2;
                y1 = w + beta * y2;
                u1 = A * y1;
                v = u1 + beta * (u2 + beta * v);
                residual = VectorMath::norm(v) / normb;
            }

            // scheme did not converge
            return std::make_tuple(false, x, maxIterations, 0);
        }

        static Return_t MINRES(SparseMatrix2D const & A, Vector const & b, int max_iterations = 10000) {
            return Minres::solve(A, b, max_iterations, 1E-15);
        }

        static Return_t MINRESLanPro(SparseMatrix2D const & A, Vector const & b, int max_iterations = 10000) {
            return MinresLanPro::solve(A, b, max_iterations, 1E-15);
        }

        static Return_t GMRES(SparseMatrix2D const & A, Vector const & b, SparseMatrix2D::size_type m, int maxIterations = 10000) {
           return Gmres::solve(A, b, m, maxIterations, 1E-15);
        }

        static Return_t GCR(SparseMatrix2D const & A, Vector const & b, SparseMatrix2D::size_type m, int maxIterations = 10000) {
            return GCR::solve(A, b, m, maxIterations, 1E-14);
        }

    };

} // LinearSolverLibrary_NS
