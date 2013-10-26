#include "pch.h"

#include "CoarseVariableSetDecorator.h"
#include "CoarseVariableSetDecoratorIteratorLogic.h"
#include "common/InputIterator.hpp"
#include "common/IInputIteratorLogic.h"

using namespace LinearSolverLibrary_NS;
using namespace common_NS;


CoarseVariableSetDecorator::CoarseVariableSetDecorator(IVariableSet const & variable_set, VariableCategorizer const & categorizer)
    : variable_set_(variable_set), categorizer_(categorizer) {}

std::function<bool(CoarseVariableSetDecorator::size_type)>
CoarseVariableSetDecorator::predicate() const {
    return [&](size_type variable) -> bool {
        return categorizer_.GetType(variable) == VariableCategorizer::Type::COARSE;
    };
}

bool
CoarseVariableSetDecorator::contains(size_type variable) const {
    if (variable_set_.contains(variable) == false)
        return false;
    return predicate()(variable);
}

CoarseVariableSetDecorator::size_type
CoarseVariableSetDecorator::size() const {
    size_type count = std::count_if(std::begin(variable_set_), std::end(variable_set_), predicate());
    return count;
}

namespace {

    template<typename U, typename T>
    std::unique_ptr<U> unique_pointer_cast(std::unique_ptr<T> && in) {
        return std::unique_ptr<U>(std::move(in));
    }

}

InputIterator<CoarseVariableSetDecorator::size_type>
CoarseVariableSetDecorator::begin() const {
    std::unique_ptr<CoarseVariableSetDecoratorIteratorLogic> logic(new CoarseVariableSetDecoratorIteratorLogic(std::begin(variable_set_), std::end(variable_set_), predicate()));
    return InputIterator_t(unique_pointer_cast<IInputIteratorLogic<size_type>>(std::move(logic)));
}

InputIterator<CoarseVariableSetDecorator::size_type>
CoarseVariableSetDecorator::end() const {
    std::unique_ptr<CoarseVariableSetDecoratorIteratorLogic> logic(new CoarseVariableSetDecoratorIteratorLogic(std::end(variable_set_), std::end(variable_set_), predicate()));
    return InputIterator_t(unique_pointer_cast<IInputIteratorLogic<size_type>>(std::move(logic)));
}
