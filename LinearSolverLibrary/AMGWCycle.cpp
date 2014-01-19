#include "pch.h"

#include "AMGWCycle.h"

#include <memory>


using namespace LinearSolverLibrary_NS;


AMGWCycle::AMGWCycle(short gamma, size_t max_depth)
    :
    gamma_{gamma},
    max_depth_{max_depth},
    gamma2_{0} {

    levels_to_ascend_ = 1;

    initialize();
}

namespace {

    template<typename U, typename T>
    std::unique_ptr<U> unique_pointer_cast(std::unique_ptr<T> && in) {
        return std::unique_ptr<U>(std::move(in));
    }

}

void
AMGWCycle::initialize() {
    auto decend_state = std::make_unique<DescendState>(*this, 0);
    setState(unique_pointer_cast<IState>(std::move(decend_state)));
}

void
AMGWCycle::setState(IStatePtr state) {
    state_ = std::move(state);
}

size_t
AMGWCycle::currentLevel() const {
    return state_->currentLevel();
}

size_t
AMGWCycle::setNextLevel() const {
    return state_->setNextLevel();
}
