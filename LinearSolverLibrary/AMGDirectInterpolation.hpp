/*
* Name  : AMGDirectInterpolation
* Path  :
* Use   : Compute prolongation/restriction operator as
          described in Trottenberg, page 479, A.7.2.1
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once


#include "VariableCategorizer.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    template<typename CoarseningStrategy>
    class AMGDirectInterpolation {
    public:
        AMGDirectInterpolation(LinAlg_NS::SparseMatrix2D const & m) : m_(m) {}

        AMGDirectInterpolation(AMGDirectInterpolation const &) = delete;
        AMGDirectInterpolation & operator=(AMGDirectInterpolation const &) = delete;

        std::tuple<LinAlg_NS::SparseMatrix2D, LinAlg_NS::SparseMatrix2D> computeInterpolationOperator() const {
            // compute C/F splitting
            CoarseningStrategy coarsening_strategy{m};
            coarsening_strategy.coarsen();

            // Compute interpolation and restriction operators
            SparseMatrix2D interpolation;
            SparseMatrix2D restriction;
        }

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
