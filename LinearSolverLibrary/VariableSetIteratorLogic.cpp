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

namespace {

    template<typename U, typename T>
    std::unique_ptr<U> unique_pointer_cast(std::unique_ptr<T> & in) {
        return std::unique_ptr<U>(std::move(in));
    }

}

std::unique_ptr<common_NS::IIteratorLogic<IVariableSet::size_type>>
VariableSetIteratorLogic::end() const {
    std::unique_ptr<VariableSetIteratorLogic> end_logic(new VariableSetIteratorLogic(variable_set_));
    end_logic->it_ = std::end(variable_set_.data_);
    return unique_pointer_cast<common_NS::IIteratorLogic<IVariableSet::size_type>>(end_logic);
}

bool
VariableSetIteratorLogic::equalTo(IIteratorLogic<IVariableSet::size_type> const & in) const {
    VariableSetIteratorLogic const * this_ = dynamic_cast<VariableSetIteratorLogic const *>(&in);
    if (this_ == nullptr)
        return false;
    return it_ == this_->it_;
}
