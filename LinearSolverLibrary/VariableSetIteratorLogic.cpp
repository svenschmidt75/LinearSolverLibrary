#include "pch.h"

#include "VariableSetIteratorLogic.h"


using namespace LinearSolverLibrary_NS;

VariableSetIteratorLogic::VariableSetIteratorLogic(VariableSet const & variable_set)
    :
    variable_set_(variable_set),
    it_(std::begin(variable_set_.data_)) {}

VariableSetIteratorLogic &
VariableSetIteratorLogic::operator=(VariableSetIteratorLogic const & in) {
    VariableSet & variable_set = const_cast<VariableSet &>(variable_set_);
    variable_set = in.variable_set_;
    return *this;
}

bool
VariableSetIteratorLogic::isValid() const {
    return it_ != std::end(variable_set_.data_);
}

void
VariableSetIteratorLogic::next() {
    ++it_;
}

IVariableSet::size_type
VariableSetIteratorLogic::get() const {
    return *it_;
}