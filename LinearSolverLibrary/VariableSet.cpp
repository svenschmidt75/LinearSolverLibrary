#include "pch.h"

#include "VariableSet.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
VariableSet::add(size_type variable) {
    data_.insert(variable);
}

bool
VariableSet::contains(size_type variable) const {
    auto it = data_.find(variable);
    return it != std::end(data_);
}

VariableSet::size_type
VariableSet::size() const {
    return data_.size();
}
