/*
* Name  : IAMGCycle
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once


namespace LinearSolverLibrary_NS {


    class IAMGCycle {
    public:
        virtual ~IAMGCycle() {}

        virtual void initialize(short max_level) const = 0;
        virtual int  currentLevel() const = 0;
        virtual void setNextLevel() const = 0;
        virtual void print() const = 0;
    };

} // LinearSolverLibrary_NS
