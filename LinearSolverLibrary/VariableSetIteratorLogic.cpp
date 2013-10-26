#include "pch.h"

#include "VariableSetIteratorLogic.h"


using namespace LinearSolverLibrary_NS;

VariableSetIteratorLogic::VariableSetIteratorLogic(VariableSet::DataIterator_t iterator)
    :
    iterator_(iterator) {}

VariableSetIteratorLogic::size_type
VariableSetIteratorLogic::get() const {
    return *iterator_;
}

void
VariableSetIteratorLogic::next() {
    ++iterator_;
}

bool
VariableSetIteratorLogic::equal(IInputIteratorLogic const & in) const {
    // No need for dynamic_cast here as this must only be called by
    // ForwardIterator for the SAME iterator type, i.e.
    // it1 !=/== it2, where it1 and it2 must have the same type.
    auto other = static_cast<VariableSetIteratorLogic const *>(&in);
    return iterator_ == other->iterator_;
}

std::unique_ptr<VariableSetIteratorLogic::This_t>
VariableSetIteratorLogic::clone() const {
    // uses default copy constructor
    auto clone = std::unique_ptr<This_t>(new VariableSetIteratorLogic(*this));
    return clone;
}
