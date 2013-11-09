#include "pch.h"

#include "AMGStandardSplitting.h"
#include "LinearSolverLibrary/VariableCardinalityPolicy.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


AMGStandardSplitting::AMGStandardSplitting(SparseMatrix2D const & m, IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer const & categorizer)
    :
    m_(m),
    variable_influence_accessor_(variable_influence_accessor),
    categorizer_(categorizer) {}

void
AMGStandardSplitting::generateSplitting() {
    initializeVariableCardinality();
}

AMGStandardSplitting::Queue_t
AMGStandardSplitting::initializeVariableCardinality() {
    Queue_t queue;
    VariableCardinalityPolicy cardinalityPolicy(variable_influence_accessor_);
    for (auto variable = 0; variable < m_.rows(); ++variable) {
        auto cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
        queue.push({cardinality, variable});
    }
    return queue;
}
