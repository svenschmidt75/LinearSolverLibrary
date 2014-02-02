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
        std::unique_ptr<LinAlg_NS::SparseMatrix2D> prolongator;
        std::unique_ptr<LinAlg_NS::SparseMatrix2D> interpolator;
        std::unique_ptr<LinAlg_NS::SparseMatrix2D> m;
        std::unique_ptr<LinAlg_NS::Vector>         f;
        std::unique_ptr<LinAlg_NS::Vector>         x;
    };

} // LinearSolverLibrary_NS
