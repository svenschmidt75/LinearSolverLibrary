#include "pch.h"

#include "VariableSet.h"
#include "VariableSetIteratorLogic.h"
#include "common/Iterator.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

bool
VariableSet::contains(size_type variable) const {
    auto it = data_.find(variable);
    return it != std::end(data_);
}

common_NS::Iterator<IVariableSet::size_type>
VariableSet::GetIterator() const {
    std::unique_ptr<common_NS::IIteratorLogic<IVariableSet::size_type>> logic(new VariableSetIteratorLogic(*this));
    return common_NS::Iterator<IVariableSet::size_type>(std::move(logic));
}

VariableSet::size_type
VariableSet::size() const {
    return data_.size();
}

void
VariableSet::add(size_type variable) {
    data_.insert(variable);
}
