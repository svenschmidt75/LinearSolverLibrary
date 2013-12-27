#include "pch.h"

#include "VariableSet.h"
#include "VariableSetIteratorLogic.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace common_NS;


bool
VariableSet::contains(size_type variable) const {
    auto it = data_.find(variable);
    return it != std::end(data_);
}

VariableSet::size_type
VariableSet::size() const {
    return data_.size();
}

namespace {

    template<typename U, typename T>
    std::unique_ptr<U> unique_pointer_cast(std::unique_ptr<T> && in) {
        return std::unique_ptr<U>(std::move(in));
    }

}

VariableSet::InputIterator_t
VariableSet::begin() const {
    std::unique_ptr<VariableSetIteratorLogic> logic(new VariableSetIteratorLogic(std::begin(data_)));
    return InputIterator<IVariableSet::size_type>(unique_pointer_cast<IInputIteratorLogic<IVariableSet::size_type>>(std::move(logic)));
}

VariableSet::InputIterator_t
VariableSet::end() const {
    std::unique_ptr<VariableSetIteratorLogic> logic(new VariableSetIteratorLogic(std::end(data_)));
    return InputIterator<IVariableSet::size_type>(unique_pointer_cast<IInputIteratorLogic<IVariableSet::size_type>>(std::move(logic)));
}

void
VariableSet::add(size_type variable) {
    data_.insert(variable);
}
