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
        LinAlg_NS::SparseMatrix2D prolongator;
        LinAlg_NS::SparseMatrix2D interpolator;
        LinAlg_NS::SparseMatrix2D m;
        LinAlg_NS::Vector         f;
        LinAlg_NS::Vector         x;
        LUDecomposition           lu_;
    };

} // LinearSolverLibrary_NS
