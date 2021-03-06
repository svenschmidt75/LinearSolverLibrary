#include "pch.h"

#include "VariableInfluenceAccessor.h"
#include "UndefinedVariableSetDecorator.h"
#include "FineVariableSetDecorator.h"
#include "IAMGStrengthPolicy.h"


using namespace LinearSolverLibrary_NS;


VariableInfluenceAccessor::VariableInfluenceAccessor(IAMGStrengthPolicy const & strength_policy, VariableCategorizer const & categorizer)
    :
    strength_policy_{strength_policy},
    categorizer_{categorizer} {}

std::unique_ptr<IVariableSet>
VariableInfluenceAccessor::GetVariableInfluencedUndefined(size_type variable) const {
    std::unique_ptr<IVariableSet> variable_set(new UndefinedVariableSetDecorator(strength_policy_.getStrongInfluencers(variable), categorizer_));
    return variable_set;
}

std::unique_ptr<IVariableSet>
VariableInfluenceAccessor::GetVariableInfluencedFine(size_type variable) const {
    std::unique_ptr<IVariableSet> variable_set(new FineVariableSetDecorator(strength_policy_.getStrongInfluencers(variable), categorizer_));
    return variable_set;
}
