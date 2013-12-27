#include "pch.h"

#include "VariableSetDecoratorBase.h"
#include "VariableSetDecoratorIteratorLogic.h"


using namespace LinearSolverLibrary_NS;
using namespace common_NS;


VariableSetDecoratorBase::VariableSetDecoratorBase(std::shared_ptr<IVariableSet> const & variable_set)
    : variable_set_(variable_set) {}

bool
VariableSetDecoratorBase::contains(size_type variable) const {
    if (variable_set_->contains(variable) == false)
        return false;
    return predicate()(variable);
}

VariableSetDecoratorBase::size_type
VariableSetDecoratorBase::size() const {
    size_type count = std::count_if(std::begin(*variable_set_), std::end(*variable_set_), predicate());
    return count;
}

namespace {

    template<typename U, typename T>
    std::unique_ptr<U> unique_pointer_cast(std::unique_ptr<T> && in) {
        return std::unique_ptr<U>(std::move(in));
    }

}

InputIterator<VariableSetDecoratorBase::size_type>
VariableSetDecoratorBase::begin() const {
    std::unique_ptr<VariableSetDecoratorIteratorLogic> logic(new VariableSetDecoratorIteratorLogic(std::begin(*variable_set_), std::end(*variable_set_), predicate()));
    return InputIterator_t(unique_pointer_cast<IInputIteratorLogic<size_type>>(std::move(logic)));
}

InputIterator<VariableSetDecoratorBase::size_type>
VariableSetDecoratorBase::end() const {
    std::unique_ptr<VariableSetDecoratorIteratorLogic> logic(new VariableSetDecoratorIteratorLogic(std::end(*variable_set_), std::end(*variable_set_), predicate()));
    return InputIterator_t(unique_pointer_cast<IInputIteratorLogic<size_type>>(std::move(logic)));
}
