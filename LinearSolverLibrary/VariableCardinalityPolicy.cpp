#include "pch.h"

#include "VariableCardinalityPolicy.h"


using namespace LinearSolverLibrary_NS;


VariableCardinalityPolicy::VariableCardinalityPolicy(IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer const & categorizer)
    : variable_influence_accessor_(variable_influence_accessor), categorizer_(categorizer) {
    initializeCardinality();
}

void
VariableCardinalityPolicy::initializeCardinality() {

}
