#include "pch.h"

#include "FineVariableSetDecorator.h"
#include "IVariableCategorizer.h"


using namespace LinearSolverLibrary_NS;
using namespace common_NS;


FineVariableSetDecorator::FineVariableSetDecorator(std::shared_ptr<IVariableSet> const & variable_set, IVariableCategorizer const & categorizer)
    :
    VariableSetDecoratorBase(variable_set),
    categorizer_{categorizer} {}

std::function<bool(FineVariableSetDecorator::size_type)>
FineVariableSetDecorator::predicate() const {
    return [&](size_type variable) -> bool {
        return categorizer_.GetType(variable) == IVariableCategorizer::Type::FINE;
    };
}
