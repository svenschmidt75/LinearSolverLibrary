/*
* Name  : PriorityArray
* Path  :
* Use   : Priority array based in binary heap implementation.
*         Note: This implementation does NOT save the priorities, rather
*         they are supplied implicitly via the comparator!
* Author: Sven Schmidt
* Date  : 01/25/2014
*/
#pragma once


#include "BinaryHeap.hpp"


namespace common_NS {


    template<typename T, template<typename> class COMPARER>
    class PriorityArray {


        friend class PriorityArrayTest;


    public:
        using size_type = std::int64_t;
        using Heap_t = std::vector<size_type>;

        using const_iterator = typename std::vector<T>::const_iterator;

    public:
        void add(T const & element) {
            heap_.add(element);
        }

        void insert(T const & element) {
            heap_.insert(element);
        }

        T const & top() const {
            return heap_.get(0);
        }

        void pop() {
            heap_.remove(0);
        }

        bool empty() const {
            return heap_.empty();
        }

    private:
        BinaryHeap<T, COMPARER> heap_;
    };

} // namespace common_NS
