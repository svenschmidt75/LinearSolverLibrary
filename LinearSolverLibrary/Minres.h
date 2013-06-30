/*
 * Name  : Minres
 * Path  : 
 * Use   : Implements sparse linear solver algorithm MINRES(m)
 * Author: Sven Schmidt
 * Date  : 06/15/2013
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/entities.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS Minres final {
    public:
        /* Return type of iterative CG solvers.
         *   bool: success or failure
         * Vector: solution x
         *    int: Number of iterations needed
         * double: Tolerance achieved
         */
        typedef std::tuple<bool, LinAlg_NS::Vector, LinAlg_NS::IMatrix2D::size_type, double> Return_t;

    public:
        static Return_t solve(LinAlg_NS::SparseMatrix2D const & A, LinAlg_NS::Vector const & b, int maxIterations, double tolerance);

    private:
        Minres();
        Return_t solve_internal(LinAlg_NS::SparseMatrix2D const & A, LinAlg_NS::Vector const & b, int maxIterations, double tolerance) const;
        void setup(LinAlg_NS::SparseMatrix2D::size_type dim, double normr) const;
        void iteration1(LinAlg_NS::SparseMatrix2D const & A) const;
        void iteration2(LinAlg_NS::SparseMatrix2D const & A) const;

    private:
        // search directions
        mutable std::vector<LinAlg_NS::Vector> p;

        // orthogonalized via Lanczos
        mutable std::vector<LinAlg_NS::Vector> q;

        // the Givens coefficients
        mutable LinAlg_NS::Vector s;
        mutable LinAlg_NS::Vector cs;
        mutable LinAlg_NS::Vector sn;
        mutable LinAlg_NS::Vector w;

        mutable LinAlg_NS::Vector r;

        mutable double beta;
        mutable double normw;
        mutable LinAlg_NS::Vector x;

        mutable double T[4];
        mutable LinAlg_NS::SparseMatrix2D::size_type k_current;
        mutable LinAlg_NS::SparseMatrix2D::size_type k_next;
        mutable LinAlg_NS::SparseMatrix2D::size_type k_prev_1;
        mutable LinAlg_NS::SparseMatrix2D::size_type k_prev_2;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
