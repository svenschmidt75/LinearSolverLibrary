#include "pch.h"

#include "VariableInfluenceAccessor.h"
#include "CoarseVariableSetDecorator.h"
#include "FineVariableSetDecorator.h"
#include "AMGStandardCoarseningStrengthPolicy.h"


using namespace LinearSolverLibrary_NS;


VariableInfluenceAccessor::VariableInfluenceAccessor(AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & categorizer)
    : strength_policy_(strength_policy), categorizer_(categorizer) {}

std::unique_ptr<IVariableSet>
VariableInfluenceAccessor::GetVariableInfluencedUndefined(size_type variable) const {
    std::unique_ptr<IVariableSet> variable_set(new CoarseVariableSetDecorator(strength_policy_.GetInfluencedByVariables(variable), categorizer_));
    return variable_set;
}

std::unique_ptr<IVariableSet>
VariableInfluenceAccessor::GetVariableInfluencedFine(size_type variable) const {
    std::unique_ptr<IVariableSet> variable_set(new FineVariableSetDecorator(strength_policy_.GetInfluencedByVariables(variable), categorizer_));
    return variable_set;
}
