/*
* Name  : Iterator
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 10/20/2013
*/
#pragma once

#include <memory>


namespace common_NS {

    template<typename T>
    class IIteratorLogic;


    template<typename T>
    class Iterator {
    public:
        Iterator(std::unique_ptr<IIteratorLogic<T>> && logic) : logic_(std::move(logic)) {}

        Iterator(Iterator const & in) : logic_(in.logic_) {}

        Iterator & operator=(Iterator const & in) {
            logic_ = in.logic_;
            return *this;
        }

        bool operator==(Iterator const & in) const {
            return logic_->equalTo(*(in.logic_));
        }

        bool operator!=(Iterator const & in) const {
            return !(*this == in);
        }

        Iterator & operator++() {
            logic_->next();
            return *this;
        }

        Iterator operator++(int) {
            Iterator this_ = *this;
            ++(*this);
            return this_;
        }

        T operator*() const {
            return logic_->get();
        }

        Iterator end() const {
            return Iterator(logic_->end());
        }

    private:
        std::unique_ptr<IIteratorLogic<T>> logic_;
    };

} // common_NS

