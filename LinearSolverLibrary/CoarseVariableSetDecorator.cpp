#include "pch.h"

#include "CoarseVariableSetDecorator.h"
#include "VariableCategorizer.h"


using namespace LinearSolverLibrary_NS;
using namespace common_NS;


CoarseVariableSetDecorator::CoarseVariableSetDecorator(std::shared_ptr<IVariableSet> const & variable_set, VariableCategorizer const & categorizer)
    :
    VariableSetDecoratorBase(variable_set),
    categorizer_(categorizer) {}

std::function<bool(CoarseVariableSetDecorator::size_type)>
CoarseVariableSetDecorator::predicate() const {
    return [&](size_type variable) -> bool {
        return categorizer_.GetType(variable) == VariableCategorizer::Type::COARSE;
    };
}
