/*
* Name  : AMGStandardInterpolationPolicy
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 05/18/2014
*/
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


// forward declarations
namespace LinearSolverLibrary_NS {
    class AMGStandardCoarseningStrengthPolicy;
    class VariableCategorizer;
}

class AMGStandardInterpolationPolicyTest_TestStrongFFConnection_Test;


namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardInterpolationPolicy {


        friend class AMGStandardInterpolationPolicyTest_TestStrongFFConnection_Test;


    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    private:
        using Interpolation_t = std::map<std::pair<size_type, size_type>, double>;

    public:
        AMGStandardInterpolationPolicy();

        AMGStandardInterpolationPolicy(AMGStandardInterpolationPolicy const &) = delete;
        AMGStandardInterpolationPolicy & operator=(AMGStandardInterpolationPolicy const &) = delete;

        bool                      Generate(LinAlg_NS::SparseMatrix2D const & m);
        LinAlg_NS::SparseMatrix2D GalerkinOperator() const;
        LinAlg_NS::SparseMatrix2D Restrictor() const;
        LinAlg_NS::SparseMatrix2D Interpolator() const;

        VariableCategorizer const & GetVariableCategorizer() const;

    private:
        bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer);
        void CreateInterpolationOperator(size_type rows, size_type columns, Interpolation_t const & interpolation_op);
        void ComputeRestrictionOperator(LinAlg_NS::SparseMatrix2D const & interpolation_operator);
        void ComputeGalerkinOperator(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::SparseMatrix2D const & interpolation_operator, LinAlg_NS::SparseMatrix2D const & restriction_operator);

    private:
        std::unique_ptr<VariableCategorizer> variable_categorizer_;
        LinAlg_NS::SparseMatrix2D            interpolation_operator_;
        LinAlg_NS::SparseMatrix2D            restriction_operator_;
        LinAlg_NS::SparseMatrix2D            galerkinOperator_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
