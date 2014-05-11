/*
* Name  : AMGVWCycleBase
* Path  : IAMGCycle
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "DeclSpec.h"

#include "IAMGCycle.h"

#include "common/IInputIteratorLogic.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {


    template<int GAMMA>
    class AMGVWCycleBase : public IAMGCycle {
    public:
        AMGVWCycleBase()
            :
            levels_index_{0} {
            static_assert(GAMMA >= 1, "GAMMA must be >= 1");
        }

        // FROM IAMGCycle
        void build(short max_level) override {
            common_NS::reporting::checkLowerBound(max_level, static_cast<short>(1));
            max_depth_ = max_level;
            levels_.push_back(0);
            generateLevel(1);
            levels_.push_back(0);
        }

        void print() const override {
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

        iterator begin() const override {
            return std::cbegin(levels_);
        }
        
        iterator end() const override {
            return std::cend(levels_);
        }

    private:
        void generateLevel(int current_level) const {
            common_NS::reporting::checkUppderBound(current_level, max_depth_);
            if (current_level == max_depth_)
                levels_.push_back(current_level);
            if (current_level < max_depth_) {
                for (short i = 0; i < GAMMA; ++i) {
                    levels_.push_back(current_level);
                    generateLevel(current_level + 1);
                }
                levels_.push_back(current_level);
            }
        }

    private:
        mutable Type_t levels_;
        mutable size_t levels_index_;
        mutable int    max_depth_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
