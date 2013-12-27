/*
 * Name  : MultiColor
 * Path  : 
 * Use   : Computes the multi-color decomposition into independent sets
 *         for a sparse matrix.
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 * 
 * Iterative Methods for Sparse Linear Systems, 2nd ed., SIAM, 2003
 * Chapter 3, page 87, Alg. 2.5 Greedy Multicoloring Algorithm
 */
#pragma once


namespace LinAlg_NS {
    class  SparseMatrix2D;
}


namespace LinearSolverLibrary_NS {

    class MatrixDecomposition;

namespace internal_NS {

    class MultiColor {
    public:
        static MatrixDecomposition apply(LinAlg_NS::SparseMatrix2D const & m);
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS
