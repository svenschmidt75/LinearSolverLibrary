/*
 * Name  : Gmres
 * Path  : 
 * Use   : Implements sparse linear solver algorithm GMRES(m)
 * Author: Sven Schmidt
 * Date  : 06/23/2013
 * 
 * Iterative Methods For solving Linear Systems, Anne Greenbaum, SIAM, 1997
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/entities.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS Gmres final {
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

    private:
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
