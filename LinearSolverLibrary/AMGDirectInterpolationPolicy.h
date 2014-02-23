/*
* Name  : AMGDirectInterpolationPolicy
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


// forward declarations
namespace LinearSolverLibrary_NS {
    class AMGStandardCoarseningStrengthPolicy;
    class VariableCategorizer;
    class VariableInfluenceAccessor;
    class AMGStandardSplitting;
}

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGDirectInterpolationPolicy {
    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    private:
        using Interpolation_t = std::map<std::pair<size_type, size_type>, double>;

    public:
        AMGDirectInterpolationPolicy();

        AMGDirectInterpolationPolicy(AMGDirectInterpolationPolicy const &) = delete;
        AMGDirectInterpolationPolicy & operator=(AMGDirectInterpolationPolicy const &) = delete;

        bool                      generate(LinAlg_NS::SparseMatrix2D const & m);
        LinAlg_NS::SparseMatrix2D GalerkinMatrix() const;
        LinAlg_NS::SparseMatrix2D Restrictor() const;
        LinAlg_NS::SparseMatrix2D Interpolator() const;

    private:
        void ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer);
        void CreateInterpolationOperator(size_type rows, size_type columns, Interpolation_t const & interpolation_op);
        void ComputeRestrictionOperator(LinAlg_NS::SparseMatrix2D const & interpolation_operator);
        void ComputeGalerkinMatrix(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::SparseMatrix2D const & interpolation_operator, LinAlg_NS::SparseMatrix2D const & restriction_operator);

    private:
        LinAlg_NS::SparseMatrix2D interpolation_operator_;
        LinAlg_NS::SparseMatrix2D prolongation_operator_;
        LinAlg_NS::SparseMatrix2D galerkinMatrix_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
