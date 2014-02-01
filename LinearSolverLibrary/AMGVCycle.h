/*
* Name  : AMGVCycle
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "DeclSpec.h"

#include "IAMGCycle.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGWCycle : public AMGVWCycleBase {
    public:
        AMGWCycle(short gamma, int max_depth);

        int  currentLevel() const;
        void setNextLevel() const;
        void print() const;

    private:
        void initialize();
        void generateLevel(int current_level);


        void FCycle();
        void FCycle_internal(short);
        void VCycle(short, short number_of_cycles);
        void move_to_deepest_level(short current_level);
        void move_from_deepest_to_current_level(short current_level);

        void FMGCycle();



    private:
        std::vector<int> levels_;
        mutable size_t   levels_index_;
        int              max_depth_;
        short            gamma_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
