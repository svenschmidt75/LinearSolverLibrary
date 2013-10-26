#include "pch.h"

#include "CoarseVariableSetDecoratorIteratorLogic.h"


using namespace LinearSolverLibrary_NS;


CoarseVariableSetDecoratorIteratorLogic::CoarseVariableSetDecoratorIteratorLogic(
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

CoarseVariableSetDecoratorIteratorLogic::size_type
CoarseVariableSetDecoratorIteratorLogic::get() const {
    return *iterator_;
}

void
CoarseVariableSetDecoratorIteratorLogic::next() {
    // Note: Must NOT be called when iterator_ == end_iterator_ already!
    while (++iterator_ != end_iterator_) {
        if (predicate_(*iterator_))
            break;
    }
}

bool
CoarseVariableSetDecoratorIteratorLogic::equal(IInputIteratorLogic const & in) const {
    // No need for dynamic_cast here as this must only be called by
    // ForwardIterator for the SAME iterator type, i.e.
    // it1 !=/== it2, where it1 and it2 must have the same type.
    auto other = static_cast<CoarseVariableSetDecoratorIteratorLogic const *>(&in);
    return iterator_ == other->iterator_;
}

std::unique_ptr<CoarseVariableSetDecoratorIteratorLogic::This_t>
CoarseVariableSetDecoratorIteratorLogic::clone() const {
    // uses default copy constructor
    auto clone = std::unique_ptr<This_t>(new CoarseVariableSetDecoratorIteratorLogic(*this));
    return clone;
}
