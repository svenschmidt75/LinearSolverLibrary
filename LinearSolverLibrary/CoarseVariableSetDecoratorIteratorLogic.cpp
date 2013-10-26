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
}

bool
CoarseVariableSetDecoratorIteratorLogic::equal(IInputIteratorLogic const & in) const {

}

std::unique_ptr<CoarseVariableSetDecoratorIteratorLogic::This_t>
CoarseVariableSetDecoratorIteratorLogic::clone() const {

}



bool
CoarseVariableSetDecoratorIteratorLogic::isValid() const {
    return it_ != it_.end();
}

void
CoarseVariableSetDecoratorIteratorLogic::next() {
    ++it_;
}

IVariableSet::size_type
CoarseVariableSetDecoratorIteratorLogic::get() const {
    return *it_;
}
