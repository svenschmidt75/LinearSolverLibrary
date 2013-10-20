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
    class Iterator {
    public:
    public:
        Iterator(IIteratorLogic<T> & logic, std::function<bool(double)> filter) : logic_(logic), filter_(filter) {}

        Iterator & operator++() {
            while (logic_.isValid()) {
                if (filter_(logic_.get()))
                    break;
                logic_.next();
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator this_ = *this;
            ++(*this);
            return this_;
        }

        double operator*() const {
            return logic_.get();
        }

    private:
        IIteratorLogic<double> &         logic_;
        std::function<bool(double)>     filter_;
    };

} // common_NS

