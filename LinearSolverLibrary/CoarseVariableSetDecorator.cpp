#include "pch.h"

#include "CoarseVariableSetDecorator.h"


using namespace LinearSolverLibrary_NS;


CoarseVariableSetDecorator::CoarseVariableSetDecorator(IVariableSet const & variable_set, VariableCategorizer const & categorizer)
    : variable_set_(variable_set), categorizer_(categorizer) {}

bool
CoarseVariableSetDecorator::contains(size_type variable) const {
    if (variable_set_.contains(variable) == false)
        return false;
    return categorizer_.GetType(variable) == VariableCategorizer::Type::COARSE;
}

CoarseVariableSetDecorator::size_type
CoarseVariableSetDecorator::size() const {
    auto filter = [&](size_type variable) -> bool {
        return categorizer_.GetType(variable) == VariableCategorizer::Type::COARSE;
    };
    size_type count = std::count_if(std::begin(variable_set_), std::end(variable_set_), filter);
    return count;
}

Iterator
CoarseVariableSetDecorator::begin() const {
    auto filter = [&](size_type variable) -> bool {
        return categorizer_.GetType(variable) == VariableCategorizer::Type::COARSE;
    };
    CoarseVariableSetIteratorLogic logic(variable_set_, filter);
    return Iterator(logic);
}

CoarseVariableSetDecorator::Iterator
CoarseVariableSetDecorator::end() const {
    return variable_set_.end();
}
