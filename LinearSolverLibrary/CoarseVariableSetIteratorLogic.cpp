#include "pch.h"

#include "CoarseVariableSetIteratorLogic.h"


using namespace LinearSolverLibrary_NS;


CoarseVariableSetIteratorLogic::CoarseVariableSetIteratorLogic(IVariableSet const & variable_set, std::function<bool (IVariableSet::size_type)> filter)
    :
    variable_set_(variable_set),
    filter_(filter),
    it_(variable_set_.GetIterator()) {}

bool
CoarseVariableSetIteratorLogic::isValid() const {
//    return it_ != it.end();
    return true;
}

void
CoarseVariableSetIteratorLogic::next() {
    ++it_;
}

IVariableSet::size_type
CoarseVariableSetIteratorLogic::get() const {
    return *it_;
}
