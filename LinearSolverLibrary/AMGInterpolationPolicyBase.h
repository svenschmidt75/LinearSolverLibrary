/*
 * Name  : AMGInterpolationPolicyBase
 * Path  :
 * Use   :
 * Author: Sven Schmidt
 * Date  : 06/14/2014
 */
#pragma once


#include "DeclSpec.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IAMGStandardStrengthPolicy;
    class IVariableCategorizer;
}


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGInterpolationPolicyBase {
    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        AMGInterpolationPolicyBase();

        AMGInterpolationPolicyBase(AMGInterpolationPolicyBase const &) = delete;
        AMGInterpolationPolicyBase & operator=(AMGInterpolationPolicyBase const &) = delete;

        bool                      Generate(LinAlg_NS::SparseMatrix2D const & m);
        LinAlg_NS::SparseMatrix2D GalerkinOperator() const;
        LinAlg_NS::SparseMatrix2D Restrictor() const;
        LinAlg_NS::SparseMatrix2D Interpolator() const;

        IVariableCategorizer const & GetVariableCategorizer() const;

    protected:
        using Interpolation_t = std::map < std::pair<size_type, size_type>, double > ;

    protected:
        void CreateInterpolationOperator(size_type rows, size_type columns, Interpolation_t const & interpolation_op);

    protected:
        std::unique_ptr<IVariableCategorizer> variable_categorizer_;
        LinAlg_NS::SparseMatrix2D             interpolation_operator_;
        LinAlg_NS::SparseMatrix2D             restriction_operator_;
        LinAlg_NS::SparseMatrix2D             galerkinOperator_;

    private:
        virtual bool ComputeInterpolationOperator(LinAlg_NS::SparseMatrix2D const & m_, IAMGStandardStrengthPolicy const & strength_policy, IVariableCategorizer const & variable_categorizer) = 0;

        void ComputeRestrictionOperator(LinAlg_NS::SparseMatrix2D const & interpolation_operator);
        void ComputeGalerkinOperator(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::SparseMatrix2D const & interpolation_operator, LinAlg_NS::SparseMatrix2D const & restriction_operator);
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
