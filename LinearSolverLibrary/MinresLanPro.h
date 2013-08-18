/*
 * Name  : MinresLanPro
 * Path  : 
 * Use   : Implements sparse linear solver algorithm MINRES, with partial
 *         reorthogonalization.
 * Author: Sven Schmidt
 * Date  : 07/02/2013
 * 
 * Iterative Methods For solving Linear Systems, Anne Greenbaum, SIAM, 1997
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/entities.h"
#include "lanczosPRO.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS MinresLanPro final {
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
        MinresLanPro();
        Return_t solve_internal(LinAlg_NS::SparseMatrix2D const & A, LinAlg_NS::Vector const & b, int maxIterations, double tolerance) const;
        void setup(LinAlg_NS::SparseMatrix2D::size_type dim, double normr) const;
        void iteration1() const;
        void iteration2() const;

    private:
        LanczosPRO lanczos;

        // search directions
        mutable std::vector<LinAlg_NS::Vector> p;

        // alphas and bets in T_i
        mutable std::vector<LinAlg_NS::Vector> a;
        mutable std::vector<LinAlg_NS::Vector> b;

        // the Givens coefficients
        mutable LinAlg_NS::Vector s;
        mutable LinAlg_NS::Vector cs;
        mutable LinAlg_NS::Vector sn;
        mutable LinAlg_NS::Vector w;

        mutable LinAlg_NS::Vector r;

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
