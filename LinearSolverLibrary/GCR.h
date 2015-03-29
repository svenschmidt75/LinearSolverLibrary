/*
 * Name  : GCR
 * Path  : 
 * Use   : Generalized Conjugate Residual
 * Author: Sven Schmidt
 * Date  : 08/24/2013
 * 
 * Generalized conjugate residual method, similar to GMRES.
 * Iterative Methods for Sparse Linear Systems, Yousef Saad, 2nd ed, 2003
 * p. 198, alg. 6.21
 */
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

    class LINEAR_SOLVER_LIBRARY_DECL_SYMBOLS GCR final : boost::noncopyable {
    public:
        /* Return type of iterative CG solvers.
         *   bool: success or failure
         * Vector: solution x
         *    int: Number of iterations needed
         * double: Tolerance achieved
         */
        typedef std::tuple<bool, LinAlg_NS::Vector, LinAlg_NS::IMatrix2D::size_type, double> Return_t;

    public:
        static Return_t solve(LinAlg_NS::SparseMatrix2D const & A, LinAlg_NS::Vector const & b, LinAlg_NS::SparseMatrix2D::size_type m, int maxIterations, double tolerance);
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
