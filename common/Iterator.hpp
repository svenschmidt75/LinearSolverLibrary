/*
* Name  : Iterator
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 10/20/2013
*/
#pragma once

namespace common_NS {

    template<typename T>
    class IIteratorLogic;


    template<typename T>
    class Iterator {
    public:
        Iterator(IIteratorLogic<T> & logic) : logic_(logic) {}

        Iterator(Iterator const & in) : logic_(in.logic_) {}

        Iterator & operator=(Iterator const & in) {
            logic_ = in.logic_;
            return *this;
        }

        Iterator & operator++() {
            logic_.next();
            return *this;
        }

        Iterator operator++(int) {
            Iterator this_ = *this;
            ++(*this);
            return this_;
        }

        T operator*() const {
            return logic_.get();
        }

    private:
        IIteratorLogic<T> & logic_;
    };

} // common_NS

