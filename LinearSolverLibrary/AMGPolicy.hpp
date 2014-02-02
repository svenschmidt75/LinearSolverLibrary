/*
* Name  : AMGPolicy
* Path  :
* Use   : Generates AMG hierarchy
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once


namespace LinearSolverLibrary_NS {

    template<typename CoarseningStrategy, typename InterpolationStrategy>
    class AMGPolicy {
    public:
        AMGPolicy(LinAlg_NS::SparseMatrix2D const & m) : m_{m} {}

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
    };

} // LinearSolverLibrary_NS
