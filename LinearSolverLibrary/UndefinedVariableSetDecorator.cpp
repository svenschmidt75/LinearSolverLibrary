#include "pch.h"

#include "UndefinedVariableSetDecorator.h"
#include "IVariableCategorizer.h"


using namespace LinearSolverLibrary_NS;
using namespace common_NS;


UndefinedVariableSetDecorator::UndefinedVariableSetDecorator(std::shared_ptr<IVariableSet> const & variable_set, IVariableCategorizer const & categorizer)
    :
    VariableSetDecoratorBase(variable_set),
    categorizer_(categorizer) {}

std::function<bool(UndefinedVariableSetDecorator::size_type)>
UndefinedVariableSetDecorator::predicate() const {
    return [&](size_type variable) -> bool {
        return categorizer_.GetType(variable) == IVariableCategorizer::Type::UNDEFINED;
    };
}
