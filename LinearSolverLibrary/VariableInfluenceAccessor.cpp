#include "pch.h"

#include "VariableInfluenceAccessor.h"
#include "CoarseVariableSetDecorator.h"


using namespace LinearSolverLibrary_NS;


VariableInfluenceAccessor::VariableInfluenceAccessor(AMGStandardCoarseningStrengthPolicy const & strength_policy)
    : strength_policy_(strength_policy) {}

std::unique_ptr<IVariableSet>
VariableInfluenceAccessor::GetVariableInfluencedUndefined(size_type variable) const {
}

std::unique_ptr<IVariableSet>
VariableInfluenceAccessor::GetVariableInfluencedFine(size_type variable) const {
    std::unique_ptr<IVariableSet> variable_set(new CoarseVariableSetDecorator());
}
