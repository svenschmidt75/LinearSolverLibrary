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
        AMGWCycle(short gamma, size_t max_depth);

        size_t currentLevel() const;
        size_t setNextLevel() const;
        void   print() const;

    private:
        void initialize();

    private:
        std::vector<size_t> levels_;
        size_t              max_depth_;
        short               gamma_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
