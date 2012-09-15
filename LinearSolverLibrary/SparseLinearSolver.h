/*
 * Name  : SparseLinearSolver
 * Path  : 
 * Use   : Implements various sparse linear solver codes
 * Author: Sven Schmidt
 * Date  : 09/15/2012
 */
#pragma once

#include "DeclSpec.h"

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS SparseLinearSolver {
    public:
        static std::tuple<bool, LinAlg_NS::Vector, int> sparseSOR(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & f, double omega, int max_iterations = 10000);
    };

} // LinearSolverLibrary_NS
