/*
* Name  : PriorityArray
* Path  :
* Use   : Priority array based in binary heap implementation.
*         Note: This implementation does NOT save the priorities, rather
*         they are supplied implicitly via the comparator!
*         After changing the priorities outside the priority array, you MUST
*         update all elements accordingly for the priority array to be in a 
*         consistent state.
* Author: Sven Schmidt
* Date  : 01/25/2014
*/
#pragma once


#include "BinaryHeap.hpp"


namespace common_NS {


    template<typename T, template<typename> class COMPARATOR>
    class PriorityArray {


        friend class PriorityArrayTest;


    public:
        using size_type = typename BinaryHeap<T, COMPARATOR>::size_type;

    public:
        PriorityArray() {}
        PriorityArray(COMPARATOR<T> const & comparator) : heap_{comparator} {}

        void add(T const & element) {
            heap_.add(element);
        }

        void insert(T const & element) {
            heap_.insert(element);
        }

        T const & top() const {
            return heap_.getElement(0);
        }

        void pop() {
            heap_.remove(0);
        }

        bool empty() const {
            return heap_.empty();
        }

        void updatePriority(size_type node_index) {
            heap_.update(node_index);
        }

    private:
        BinaryHeap<T, COMPARATOR> heap_;
    };

} // namespace common_NS
