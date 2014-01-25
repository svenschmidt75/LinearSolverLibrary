/*
* Name  : BinaryHeap
* Path  :
* Use   : Binary heap implementation.
* Author: Sven Schmidt
* Date  : 01/24/2013
*/
#pragma once



namespace common_NS {


    template<typename T, template<typename> class COMPARER>
    class BinaryHeap {

        friend class BinaryHeapTest;


    public:
        using Heap_t = std::vector<int>;
        using size_type = std::int64_t;

    public:
        BinaryHeap() : heaped_{false} {}

        void add(T const & element) {
            // add element respecting the shape property, i.e.
            // add the left child first
            keys_.push_back(element);
            heaped_ = false;
        }

        T const & top(T const & element) {
            // swap last element with 1st
            if (heap_.empty() == false) {
                if (heaped_ == false)
                    build_heap();
                return keys_[heap_[0]];
            }
        }

        void pop() {
            // swap last element with 1st
            if (heap_.empty() == false) {
                if (heaped_ == false)
                    build_heap();
                std::swap(&heap_[0], &heap_[heap_.size() - 1]);
                heap_.pop_back();
                heapify(0);
            }
        }

        //    private:
        void heapify(size_type parent_node_index) {
            auto heap_size = static_cast<size_type>(heap_.size());
            auto left_child_index = 2 * parent_node_index + 1;
            auto right_child_index = left_child_index + 1;
            auto i = parent_node_index;
            auto priority = [this](size_type node_index) {
                return keys_[node_index];
            };
            if (left_child_index < heap_size && std::greater<T>()(priority(left_child_index), priority(parent_node_index)))
                // swap left child with its parent
                i = left_child_index;
            if (right_child_index < heap_size && std::greater<T>()(priority(right_child_index), priority(i)))
                // swap right child with its parent
                i = right_child_index;
            if (i != parent_node_index) {
                std::swap(heap_[parent_node_index], heap_[i]);
                heapify(i);
            }
        }

        void build_heap() {
            auto heap_size = keys_.size();
            heap_.resize(heap_size);
            std::iota(std::begin(heap_), std::end(heap_), 0);
            for (size_type i = heap_size / 2 - 1; i >= 0; --i)
                heapify(i);
            heaped_ = true;
        }

    private:
        bool           heaped_;
        Heap_t         heap_;
        std::vector<T> keys_;
    };

} // namespace common_NS
