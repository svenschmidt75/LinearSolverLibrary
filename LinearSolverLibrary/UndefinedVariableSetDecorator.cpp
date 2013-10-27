#include "pch.h"

#include "UndefinedVariableSetDecorator.h"
#include "VariableCategorizer.h"


using namespace LinearSolverLibrary_NS;
using namespace common_NS;


UndefinedVariableSetDecorator::UndefinedVariableSetDecorator(std::shared_ptr<IVariableSet> const & variable_set, VariableCategorizer const & categorizer)
    :
    VariableSetDecoratorBase(variable_set),
    categorizer_(categorizer) {}

std::function<bool(UndefinedVariableSetDecorator::size_type)>
UndefinedVariableSetDecorator::predicate() const {
    return [&](size_type variable) -> bool {
        return categorizer_.GetType(variable) == VariableCategorizer::Type::UNDEFINED;
    };
}
