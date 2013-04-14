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
#include "LinAlg/UHMatrix.h"

#include <tuple>


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
            return std::make_tuple(false, x, 10000, 0);
        }

        static Return_t BiCG_old(SparseMatrix2D const & m, SparseMatrix2D const & m_transposed, Vector const & b, int max_iterations = 10000) {
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
            double rho_prev;

            for (SparseMatrix2D::size_type i = 0; i < max_iterations; ++i) {
                rho = VectorMath::dotProduct(r1, r2);
                if (!rho) {
                    // r1 and r2 orthogonal
                    double residual = VectorMath::norm(r1) / normb;
                    return std::make_tuple(false, x, i, residual);
                }

                if (i) {
                    double beta = rho / rho_prev;
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

//                 std::cout << "Iteration " << i << ": " << residual << std::endl;
            }

            // scheme did not converge
            return std::make_tuple(false, x, 10000, 0);
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
            return std::make_tuple(false, x, 10000, 0);
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
            return std::make_tuple(false, x, 10000, 0);
        }


        /*
         * Computes the Givens rotation associated with scalars a and b such that
         * 
         * [  c   s ]T [a] = [r]
         * [ -s   c ]  [b]   [0]
         *
         * Reference:
         *	G. H. Golub and C. F. Van Loan, "Matrix Computations", 
         *	Johns Hopkins, Baltimore, 3rd ed. 1996.
         * 	Algorithm 5.1.3, p. 216.
         */
         static void GeneratePlaneRotation(double dx, double dy, double & cs, double & sn) {
            if (dy == 0.0) {
                cs = 1.0;
                sn = 0.0;
            } else if (abs(dy) > abs(dx)) {
                double temp = dx / dy;
                sn = 1.0 / std::sqrt(1.0 + temp * temp);
                cs = temp * sn;
            } else {
                double temp = dy / dx;
                cs = 1.0 / std::sqrt(1.0 + temp * temp);
                sn = temp * cs;
            }
        }

        static void ApplyPlaneRotation(double & dx, double & dy, double cs, double sn) {
            // apply Givens rotation
            double tmp = cs * dx + sn * dy;
            dy = -sn * dx + cs * dy;
            dx = tmp;
        }

        template<typename MATRIX>
        static void Update(Vector & x, typename MATRIX::size_type k, MATRIX const & H, Vector const & s, std::vector<Vector> const & q) {
            /* H(k + 1) (i.e. i = 0, ..., k) is upper triangular.
             * Solve H(i + 1) * y = s via back-substitution., 
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

        static Return_t GMRES(SparseMatrix2D const & A, Vector const & b, SparseMatrix2D::size_type m, int max_iterations = 10000) {
            /* Implements the GMRES(m) restarted algorithm from
             *  Reference:
             *  R. Barrett et al, "Templates for the Solution of Linear Systems:
             *  Building Blocks for Iterative Methods", SIAM, 1994.
             */
            double tol = 1E-15;

            double normb = VectorMath::norm(b);

            // guessed x = null vector
            Vector r(b);

            double beta = VectorMath::norm(r);

            double residual = beta / normb;
            if (residual <= tol)
                return std::make_tuple(true, b, 0, residual);

            auto dim = A.cols();

            // space for the orthogonal Arnoldi vectors
            std::vector<Vector> q(m + 1, Vector(dim));

            // the Givens coefficients
            Vector s(m + 1), cs(m + 1), sn(m + 1), w(dim);

            int j = 1;

            UHMatrix H(m + 1);

            Vector x(b.size());

            while (j <= max_iterations) {
                q[0] = r * (1.0 / beta);
                s(0) = beta;

                for (int i = 0; i < m && j <= max_iterations; ++i, ++j) {
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

//                     H.print();

                    // apply all previous Givens rotations to the last column
                    // of H. See Iterative Methods for Solving Linear systems,
                    // Anne Greenbaum, page 40
                    for (int k = 0; k < i; ++k)
                        // apply Givens rotation to column i, row 0, ..., i - 1
                        ApplyPlaneRotation(H(k, i), H(k + 1, i), cs(k), sn(k));

//                    H.print();

                    // compute the Givens coefficients cs and sn
                    GeneratePlaneRotation(H(i, i), H(i + 1, i), cs(i), sn(i));

                    // using the Givens coefficients cn and sn, eliminate
                    // H(i + 1, i) to make it upper triangular
                    ApplyPlaneRotation(H(i, i), H(i + 1,i), cs(i), sn(i));

//                     H.print();

                    // apply Givens rotation to r.h.s.
                    ApplyPlaneRotation(s(i), s(i + 1), cs(i), sn(i));

                    // s(k) = 0, except for k = i + 1, which equals the norm
                    // of the residual
                    residual = std::fabs(s(i + 1) / normb);
                    if (residual < tol) {
                        Update(x, i, H, s, q);

                        return std::make_tuple(true, x, j, residual);
                    }
                }

                Update(x, m - 1, H, s, q);

                r = b - A * x;
                beta = VectorMath::norm(r);

                residual = beta / normb;
                if (residual < tol)
                    return std::make_tuple(true, x, j, residual);

                // reset r.h.s. to zero
                s.clear();
            }

            // scheme did not converge
            return std::make_tuple(false, x, 10000, 0);
        }

    };

} // LinearSolverLibrary_NS
