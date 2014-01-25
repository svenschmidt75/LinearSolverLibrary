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
                std::swap(heap_[0], heap_[heap_.size() - 1]);
                heap_.pop_back();
                heapify(0);
            }
        }

    private:
        void heapify(size_type parent_node_index) {
            auto heap_size = static_cast<size_type>(heap_.size());
            auto left_child_index = 2 * parent_node_index + 1;
            auto right_child_index = left_child_index + 1;
            auto i = parent_node_index;
            auto key = [this](size_type node_index) {
                return keys_[heap_[node_index]];
            };
            if (left_child_index < heap_size && COMPARER<T>()(key(left_child_index), key(parent_node_index)))
                // swap left child with its parent
                i = left_child_index;
            if (right_child_index < heap_size && COMPARER<T>()(key(right_child_index), key(i)))
                // swap right child with its parent
                i = right_child_index;
            if (i != parent_node_index) {
                std::swap(heap_[parent_node_index], heap_[i]);
                heapify(i);
            }
        }

        void build_heap() {
            initialize_heap();
            int tree_depth = get_tree_depth();
            for (int level = tree_depth; level > 0; --level) {
                int start_node;
                int end_node;
                std::tie(start_node, end_node) = get_node_range_for_tree_level(level);
                while (start_node < end_node) {
                    heapify(start_node);
                    start_node++;
                }
            }
            heaped_ = true;
        }

        std::tuple<int, int> get_node_range_for_tree_level(int tree_level) const {
            int start_node = static_cast<int>(std::pow(2, tree_level - 1)) - 1;
            int end_node = static_cast<int>(std::pow(2, tree_level)) - 1;
            return std::make_tuple(start_node, end_node);
        }

        void initialize_heap() {
            auto heap_size = keys_.size();
            heap_.resize(heap_size);
            std::iota(std::begin(heap_), std::end(heap_), 0);
        }

        int get_tree_depth() const {
            auto heap_size = keys_.size();
            return static_cast<int>(std::log2(heap_size));
        }

    private:
        bool           heaped_;
        Heap_t         heap_;
        std::vector<T> keys_;
    };

} // namespace common_NS
