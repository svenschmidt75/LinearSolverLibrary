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
    protected:
        using Type_t = std::vector<int>;

    public:
        using iterator = Type_t::const_iterator;

    public:
        virtual ~IAMGCycle() {}

        virtual void     build(short max_level) = 0;
        virtual int      currentLevel() const = 0;
        virtual void     setNextLevel() const = 0;
        virtual void     print() const = 0;
        virtual iterator begin() const = 0;
        virtual iterator end() const = 0;
    };

} // LinearSolverLibrary_NS
