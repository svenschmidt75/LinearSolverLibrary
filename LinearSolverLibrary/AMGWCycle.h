/*
* Name  : AMGWCycle
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 01/19/2014
*/
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGWCycle {
    public:
        AMGWCycle(short gamma, int max_depth);

        int currentLevel() const;
        int setNextLevel() const;
        void   print() const;

    private:
        void initialize();
        void generateLevel(int current_level);


        void FCycle();
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
