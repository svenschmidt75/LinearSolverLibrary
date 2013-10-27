#include "pch.h"

#include "VariableCardinalityPolicy.h"


using namespace LinearSolverLibrary_NS;


VariableCardinalityPolicy::VariableCardinalityPolicy(IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer const & categorizer)
    :
    variable_influence_accessor_(variable_influence_accessor),
    categorizer_(categorizer) {
    initializeCardinality();
}

void
VariableCardinalityPolicy::initializeCardinality() {
}

size_t
VariableCardinalityPolicy::GetCardinalityForVariable(size_type variable) const {
    auto fineGridVariablesInfluencedBy = variable_influence_accessor_.GetVariableInfluencedFine(variable);
    auto undefinedGridVariablesInfluencedBy = variable_influence_accessor_.GetVariableInfluencedUndefined(variable);
    auto cardinality = undefinedGridVariablesInfluencedBy->size() + 2 * fineGridVariablesInfluencedBy->size();
    return cardinality;
}
