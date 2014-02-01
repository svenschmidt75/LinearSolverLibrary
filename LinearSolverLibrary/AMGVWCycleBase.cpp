#include "pch.h"

#include "AMGVWCycleBase.h"

#include <memory>


using namespace LinearSolverLibrary_NS;


AMGVWCycleBase::AMGVWCycleBase(short gamma)
    :
    gamma_{gamma},
    levels_index_{0} {
    common_NS::reporting::checkLowerBound(gamma, static_cast<short>(1));
}

void
AMGVWCycleBase::initialize(short max_level) const {
    max_depth_ = max_level;
    levels_.push_back(0);
    generateLevel(1);
    levels_.push_back(0);
}

void
AMGVWCycleBase::generateLevel(int current_level) {
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

void
AMGVWCycleBase::FCycle() {
    levels_.push_back(0);
    FCycle_internal(1);
    levels_.push_back(0);
}

void
AMGVWCycleBase::FCycle_internal(short current_level) {
    levels_.push_back(current_level);
    if (current_level == max_depth_)
        return;
    FCycle_internal(current_level + 1);
    VCycle(current_level, gamma_);
}

void
AMGVWCycleBase::VCycle(short current_level, short number_of_cycles) {
    levels_.push_back(current_level);
    for (short i = 0; i < number_of_cycles; ++i) {
        move_to_deepest_level(current_level + 1);
        move_from_deepest_to_current_level(current_level);
    }
}

void
AMGVWCycleBase::move_to_deepest_level(short current_level) {
    for (int i = current_level; i <= max_depth_; ++i)
        levels_.push_back(i);
}

void
AMGVWCycleBase::move_from_deepest_to_current_level(short current_level) {
    for (int i = max_depth_ - 1; i >= current_level; --i)
        levels_.push_back(i);
}

void
AMGVWCycleBase::FMGCycle() {
    max_depth_ = 7;

    levels_.clear();
    int current_level = max_depth_;
    int turning_level = max_depth_ - 1;
    int direction = 1;
    int number_of_turnarounds = 0;
    do {
        levels_.push_back(current_level);
        if (current_level == turning_level) {
            direction *= -1;
        }
        else if (current_level == max_depth_) {
            turning_level = max_depth_ - ++number_of_turnarounds;
            direction *= -1;
        }
        current_level += direction;
    } while (current_level > 0);

    // attach v cycle
    for (current_level = 0; current_level <= max_depth_; ++current_level)
        levels_.push_back(current_level);
    for (current_level = max_depth_ - 1; current_level >= 0; --current_level)
        levels_.push_back(current_level);
}

int
AMGVWCycleBase::currentLevel() const {
    return levels_[levels_index_];
}

int
AMGVWCycleBase::setNextLevel() const {
    common_NS::reporting::checkUppderBound(levels_index_, levels_.size() - 1);
    auto level = levels_[levels_index_];
    ++levels_index_;
    return level;
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