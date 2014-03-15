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
AMGVWCycleBase::initialize(short max_level) const {
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

namespace {

    class IteratorLogic : public common_NS::IInputIteratorLogic<int> {
    public:
        IteratorLogic(std::vector<int> const & levels, std::vector<int>::const_iterator position)
            :
            levels_{levels},
            position_{position} {}

        IteratorLogic & operator=(IteratorLogic const &) = delete;

        // FROM IInputIteratorLogic<int>
        int get() const {
            return 0;
        }

        void next() {
        }
        
        bool equal(IInputIteratorLogic const &) const{
            return false;
        }
        
        std::unique_ptr<IInputIteratorLogic<int>> clone() const {
            return nullptr;
        }

    private:
        std::vector<int> const &         levels_;
        std::vector<int>::const_iterator position_;
    };

}

namespace {

    template<typename U, typename T>
    std::unique_ptr<U> unique_pointer_cast(std::unique_ptr<T> && in) {
        return std::unique_ptr<U>(std::move(in));
    }

}

common_NS::InputIterator<int>
AMGVWCycleBase::cbegin() const {
    return common_NS::InputIterator<int>{std::make_unique<IteratorLogic>(levels_, std::cbegin(levels_))};
}

common_NS::InputIterator<int>
AMGVWCycleBase::cend() const {
    return common_NS::InputIterator<int>(std::make_unique<IteratorLogic>(levels_, std::cend(levels_)));
}
