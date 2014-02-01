/*
* Name  : AMGVWCycleBase
* Path  : IAMGCycle
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "IAMGCycle.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {


    class AMGVWCycleBase : public IAMGCycle {
    public:
        AMGVWCycleBase(short gamma);

        void initialize(short max_level) const;
        int  currentLevel() const;
        void setNextLevel() const;
        void print() const;

    private:
        void generateLevel(int current_level) const;

        void VCycle(short, short number_of_cycles);
        void move_to_deepest_level(short current_level);
        void move_from_deepest_to_current_level(short current_level);

    private:
        mutable std::vector<int> levels_;
        mutable size_t           levels_index_;
        mutable int              max_depth_;
        short                    gamma_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
