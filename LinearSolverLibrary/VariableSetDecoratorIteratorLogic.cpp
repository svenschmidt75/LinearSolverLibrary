#include "pch.h"

#include "VariableSetDecoratorIteratorLogic.h"


using namespace LinearSolverLibrary_NS;


VariableSetDecoratorIteratorLogic::VariableSetDecoratorIteratorLogic(
    common_NS::InputIterator<IVariableSet::size_type> iterator,
    common_NS::InputIterator<IVariableSet::size_type> end_iterator,
    std::function<bool(IVariableSet::size_type)> predicate)
    :
    iterator_(iterator),
    end_iterator_(end_iterator),
    predicate_(predicate) {

    // jump to 1st element
    while (iterator_ != end_iterator_) {
        if (predicate_(*iterator_))
            break;
        ++iterator_;
    }
}

VariableSetDecoratorIteratorLogic::size_type
VariableSetDecoratorIteratorLogic::get() const {
    return *iterator_;
}

void
VariableSetDecoratorIteratorLogic::next() {
    // Note: Must NOT be called when iterator_ == end_iterator_ already!
    while (++iterator_ != end_iterator_) {
        if (predicate_(*iterator_))
            break;
    }
}

bool
VariableSetDecoratorIteratorLogic::equal(IInputIteratorLogic const & in) const {
    // No need for dynamic_cast here as this must only be called by
    // ForwardIterator for the SAME iterator type, i.e.
    // it1 !=/== it2, where it1 and it2 must have the same type.
    auto other = static_cast<VariableSetDecoratorIteratorLogic const *>(&in);
    return iterator_ == other->iterator_;
}

std::unique_ptr<VariableSetDecoratorIteratorLogic::This_t>
VariableSetDecoratorIteratorLogic::clone() const {
    // uses default copy constructor
    auto clone = std::unique_ptr<This_t>(new VariableSetDecoratorIteratorLogic(*this));
    return clone;
}
