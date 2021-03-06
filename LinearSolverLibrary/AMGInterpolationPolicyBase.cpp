#include "pch.h"

#include "AMGInterpolationPolicyBase.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGInterpolationPolicyBase::AMGInterpolationPolicyBase() {}

bool
AMGInterpolationPolicyBase::Generate(SparseMatrix2D const & m, IAMGStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) {
    if (ComputeInterpolationOperator(m, strength_policy, variable_categorizer) == false)
        return false;
    ComputeRestrictionOperator(interpolation_operator_);
    ComputeGalerkinOperator(m, interpolation_operator_, restriction_operator_);
    return true;
}

void
AMGInterpolationPolicyBase::CreateInterpolationOperator(size_type rows, size_type columns, Interpolation_t const & interpolation_op) {
    interpolation_operator_ = SparseMatrix2D{rows, columns};
    for (auto const & item : interpolation_op) {
        auto fine_variable = item.first.first;
        auto coarse_variable = item.first.second;
        auto value = item.second;
        interpolation_operator_(fine_variable, coarse_variable) = value;
    }
    interpolation_operator_.finalize();
//    interpolation_operator_.print();
}

void
AMGInterpolationPolicyBase::ComputeRestrictionOperator(SparseMatrix2D const & interpolation_operator) {
    restriction_operator_ = helper::transpose(interpolation_operator);
}

void
AMGInterpolationPolicyBase::ComputeGalerkinOperator(SparseMatrix2D const & m, SparseMatrix2D const & interpolation_operator, SparseMatrix2D const & restriction_operator) {
    galerkinOperator_ = helper::matrixMul(restriction_operator, helper::matrixMul(m, interpolation_operator));
    //Matrix2D m1 = helper::SparseToDense(m);
    //Matrix2D m2 = helper::SparseToDense(interpolation_operator);
    //Matrix2D m3 = helper::SparseToDense(restriction_operator);
    //galerkinOperator_ = helper::DenseToSparse(helper::matrixMul(m3, helper::matrixMul(m1, m2)));
}

SparseMatrix2D
AMGInterpolationPolicyBase::GalerkinOperator() const {
    return galerkinOperator_;
}

SparseMatrix2D
AMGInterpolationPolicyBase::Restrictor() const {
    return restriction_operator_;
}

SparseMatrix2D
AMGInterpolationPolicyBase::Interpolator() const {
    return interpolation_operator_;
}
