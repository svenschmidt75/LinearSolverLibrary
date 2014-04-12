/*
* Name  : AMGLevel
* Path  :
* Use   : Describes one level in an AMG hierarchy
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once


namespace LinearSolverLibrary_NS {

    struct AMGLevel {
        std::shared_ptr<LinAlg_NS::SparseMatrix2D> restrictor;
        std::shared_ptr<LinAlg_NS::SparseMatrix2D> interpolator;
        LinAlg_NS::SparseMatrix2D                  m;
        LinAlg_NS::Vector                          f;
        LinAlg_NS::Vector                          x;
        MatrixDecomposition                        variableDecomposition;
    };

} // LinearSolverLibrary_NS
