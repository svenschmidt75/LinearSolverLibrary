#include "pch.h"

#include "AMGStandardSplitting.h"
#include "LinearSolverLibrary/VariableCardinalityPolicy.h"
#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinearSolverLibrary/VariableInfluenceAccessor.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


AMGStandardSplitting::AMGStandardSplitting(SparseMatrix2D const & m, IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer & categorizer)
    :
    m_{m},
    variable_influence_accessor_{variable_influence_accessor},
    categorizer_{categorizer} {}

void
AMGStandardSplitting::generateSplitting() {
    queue_ = initializeVariableCardinality();
    while (queue_.empty() == false) {
        auto const item = queue_.top();
        queue_.pop();
        auto variable = item.second;
        if (categorizer_.GetType(variable) != VariableCategorizer::Type::UNDEFINED)
            continue;
        categorizer_.SetType(variable, VariableCategorizer::Type::COARSE);
        categorizeVariablesStronglyInfluencing(variable);
    }
}

void
AMGStandardSplitting::categorizeVariablesStronglyInfluencing(IMatrix2D::size_type variable) {
    std::set<IMatrix2D::size_type> variables;
    auto strongInfluencers = variable_influence_accessor_.GetVariableInfluencedUndefined(variable);
    for (auto otherVariable : *strongInfluencers) {
        BOOST_ASSERT_MSG(categorizer_.GetType(otherVariable) == VariableCategorizer::Type::UNDEFINED, "Variable must be of type undefined");
        categorizer_.SetType(otherVariable, VariableCategorizer::Type::FINE);
        auto const & a = variable_influence_accessor_.GetVariableInfluencedUndefined(otherVariable);
        variables.insert(std::begin(*a), std::end(*a));
    }
    adjustCardinalityOfStrongInfluencers(variables);
}

void 
AMGStandardSplitting::adjustCardinalityOfStrongInfluencers(std::set<IMatrix2D::size_type> const & variables) {
    VariableCardinalityPolicy cardinalityPolicy(variable_influence_accessor_);
    for (auto otherVariable : variables) {
        if (categorizer_.GetType(otherVariable) == VariableCategorizer::Type::UNDEFINED) {
            auto cardinality = cardinalityPolicy.GetCardinalityForVariable(otherVariable);
            queue_.emplace(cardinality, otherVariable);
        }
    }
}

AMGStandardSplitting::Queue_t
AMGStandardSplitting::initializeVariableCardinality() {
    Queue_t queue;
    VariableCardinalityPolicy cardinalityPolicy(variable_influence_accessor_);
    for (auto variable = 0; variable < m_.rows(); ++variable) {
        auto cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
        queue.emplace(cardinality, variable);
    }
    return queue;
}
