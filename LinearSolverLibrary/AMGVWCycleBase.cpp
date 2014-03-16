#include "pch.h"

#include "AMGVWCycleBase.h"

#include "common/IInputIteratorLogic.h"

#include <memory>


using namespace LinearSolverLibrary_NS;


AMGVWCycleBase::AMGVWCycleBase(short gamma)
    :
    gamma_{gamma},
    levels_index_{0} {
    common_NS::reporting::checkLowerBound(gamma, static_cast<short>(1));
}

void
AMGVWCycleBase::build(short max_level) {
    common_NS::reporting::checkLowerBound(max_level, static_cast<short>(1));
    max_depth_ = max_level;
    levels_.push_back(0);
    generateLevel(1);
    levels_.push_back(0);
}

void
AMGVWCycleBase::generateLevel(int current_level) const {
    common_NS::reporting::checkUppderBound(current_level, max_depth_);
    if (current_level == max_depth_)
        levels_.push_back(current_level);
    if (current_level < max_depth_) {
        for (short i = 0; i < gamma_; ++i) {
            levels_.push_back(current_level);
            generateLevel(current_level + 1);
        }
        levels_.push_back(current_level);
    }
}

int
AMGVWCycleBase::currentLevel() const {
    return levels_[levels_index_];
}

void
AMGVWCycleBase::setNextLevel() const {
    common_NS::reporting::checkUppderBound(levels_index_ + 1, levels_.size());
    ++levels_index_;
}

void
AMGVWCycleBase::print() const {
    std::cout << std::endl;
    for (auto row = 0; row <= max_depth_; ++row) {
        for (auto column = 0; column < levels_.size(); ++column) {
            auto level = levels_[column];
            if (level == row)
                std::cout << '.';
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

AMGVWCycleBase::iterator
AMGVWCycleBase::begin() const {
    return std::cbegin(levels_);
}

AMGVWCycleBase::iterator
AMGVWCycleBase::end() const {
    return std::cend(levels_);
}
