/*
 * Name  : SparseLinearSolver
 * Path  : 
 * Use   : Implements various sparse linear solver codes
 * Author: Sven Schmidt
 * Date  : 09/15/2012
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"

#include <tuple>


namespace LinearSolverLibrary_NS {

    class MatrixDecomposition;


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS SparseLinearSolverLibrary {
    public:
        static std::tuple<bool, LinAlg_NS::Vector, int> sparseSOR(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & f, double omega, int max_iterations = 10000);
        static std::tuple<bool, LinAlg_NS::Vector, int> sparseSORMultiColor(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & f, MatrixDecomposition const & mc, double omega, int max_iterations);
    };

} // LinearSolverLibrary_NS