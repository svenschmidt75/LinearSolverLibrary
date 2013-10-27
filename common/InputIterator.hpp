/*
* Name  : InputIterator
* Path  :
* Use   : Input iterator, i.e. iterator that moves forward and permits
          read-only access.
* Author: Sven Schmidt
* Date  : 10/26/2013
*/
#pragma once

#include "IInputIteratorLogic.h"

#include <iterator>
#include <memory>

namespace common_NS {

    template<typename T>
    class InputIterator : public std::iterator<std::input_iterator_tag, T> {
    public:
        InputIterator(std::unique_ptr<IInputIteratorLogic<T>> && logic) : logic_(std::move(logic)) {}

        InputIterator(InputIterator const & in) {
            // next line: move assignment
            logic_ = std::move(in.logic_->clone());
        }

        InputIterator operator=(InputIterator const & in) {
            // next line: move assignment
            logic_ = std::move(in.logic_->clone());
            return *this;
        }

        InputIterator & operator++() {
            logic_->next();
            return *this;
        }

        InputIterator operator++(int) {
            auto this_ = *this;
            ++(*this);
            return this_;
        }

        T operator*() const {
            return logic_->get();
        }

        bool operator==(InputIterator const & in) const {
            return logic_->equal(*(in.logic_));
        }

        bool operator!=(InputIterator const & in) const {
            return !(*this == in);
        }

    private:
        std::unique_ptr<IInputIteratorLogic<T>> logic_;
    };

} // namespace common_NS
