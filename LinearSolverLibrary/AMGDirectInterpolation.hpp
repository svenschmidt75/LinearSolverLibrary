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

#include "LinAlg/SparseMatrix2D.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    template<typename AMGSplittingPolicy>
    class AMGDirectInterpolation {
    public:
        AMGDirectInterpolation(LinAlg_NS::SparseMatrix2D const & m) : m_(m) {}

        AMGDirectInterpolation(AMGDirectInterpolation const &) = delete;
        AMGDirectInterpolation & operator=(AMGDirectInterpolation const &) = delete;

        std::tuple<LinAlg_NS::SparseMatrix2D, LinAlg_NS::SparseMatrix2D> computeInterpolationOperator() const {
            // compute C/F splitting
            AMGStandardCoarseningStrengthPolicy strength_policy(m);
            VariableCategorizer variable_categorizer(m.rows());
            VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
            AMGSplittingPolicy splitting(m, influence_accessor, variable_categorizer);
            splitting.generateSplitting();

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
