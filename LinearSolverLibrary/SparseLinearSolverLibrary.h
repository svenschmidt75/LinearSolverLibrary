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

    // forward declaration
    class MatrixDecomposition;
    struct Monitor;


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS SparseLinearSolverLibrary {
    public:
        static std::tuple<bool, LinAlg_NS::Vector, int> SparseSOR(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & f, double omega, int max_iterations = 10000);
        static std::tuple<bool, LinAlg_NS::Vector, int> SparseSORMultiColor(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & f, MatrixDecomposition const & mc, double omega, int max_iterations);
        static std::tuple<bool, LinAlg_NS::Vector> SparseGSMultiColor(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & x_initial, LinAlg_NS::Vector const & f, MatrixDecomposition const & mc, Monitor & monitor);
    };

} // LinearSolverLibrary_NS
