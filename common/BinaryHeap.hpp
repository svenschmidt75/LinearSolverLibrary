/*
* Name  : BinaryHeap
* Path  :
* Use   : Binary heap implementation.
* Author: Sven Schmidt
* Date  : 01/24/2014
*/
#pragma once


namespace common_NS {


    template<typename T, template<typename> class COMPARATOR>
    class BinaryHeap {


        friend class BinaryHeapTest;
        friend class PriorityArrayTest;

    public:
        using size_type = std::int64_t;
        using Heap_t = std::vector<size_type>;

        using const_iterator = typename std::vector<T>::const_iterator;

    public:
        BinaryHeap() : BinaryHeap{COMPARATOR<T>{}} {}
        BinaryHeap(COMPARATOR<T> const & comparator) : comparator_{comparator}, heaped_{false} {}

        void add(T const & element) {
            // Add element respecting the shape property, i.e.
            // add the left child first, but do not construct
            // heap yet (faster to initialize).
            keys_.push_back(element);
            heaped_ = false;
        }

        T const & getElement(size_type node_index) const {
            common_NS::reporting::checkUppderBound(node_index + 1, static_cast<size_type>(keys_.size()));
            build_heap_if_needed();
            return keys_[heap_[node_index]];
        }

        size_type getIndexForElement(T const & element) const {
            auto it = std::find(std::cbegin(keys_), std::cend(keys_), element);
            if (it == std::cend(keys_))
                throw std::runtime_error("BinaryHeap::getIndexForElement: Element not found");
            size_type position = it - std::cbegin(keys_);
            size_type index = heap_[position];
            return index;
        }

        void insert(T const & element) {
            // Insert element while preserving the heap property.
            build_heap_if_needed();
            keys_.push_back(element);
            size_type node_index = keys_.size() - 1;
            heap_.push_back(node_index);
            swim_up(node_index);
        }

        void remove(size_type node_index) {
            // Swap the node to delete with the last one.
            common_NS::reporting::checkUppderBound(node_index + 1, static_cast<size_type>(keys_.size()));
            build_heap_if_needed();
            size_type last_node_index = keys_.size() - 1;
            std::swap(heap_[node_index], heap_[last_node_index]);
            remove_key_and_adjust_mapping(last_node_index);
            update_internal(node_index);
        }

        void update(size_type node_index) {
            common_NS::reporting::checkUppderBound(node_index + 1, static_cast<size_type>(keys_.size()));
            build_heap_if_needed();
            update_internal(node_index);
        }

        const_iterator cbegin() const {
            return std::cbegin(keys_);
        }

        const_iterator cend() const {
            return std::cend(keys_);
        }

        bool empty() const {
            build_heap_if_needed();
            return heap_.empty();
        }

    private:
        void update_internal(size_type node_index) {
            if (node_index == 0)
                heapify(0);
            else {
                auto heap_size = static_cast<size_type>(heap_.size());
                if (node_index < heap_size) {
                    size_type parent_node_index = (node_index - 1) / 2;
                    if (comparator_(key(node_index), key(parent_node_index)))
                        swim_up(node_index);
                    else if (comparator_(key(parent_node_index), key(node_index)))
                        heapify(node_index);
                }
            }
        }

        void remove_key_and_adjust_mapping(size_type node_index) {
            size_type index = heap_[node_index];
            heap_.pop_back();
            keys_.erase(std::cbegin(keys_) + index);
            // adjust mapping
            for (auto & item : heap_) {
                if (item > index)
                    --item;
            }
        }

        void build_heap_if_needed() const {
            if (keys_.empty() == false && heaped_ == false)
                build_heap();
        }

        void build_heap() const {
            initialize_heap();
            size_type heap_size = keys_.size();
            for (auto i = heap_size / 2 - 1; i >= 0; i--)
                heapify(i);
            heaped_ = true;
        }

        void initialize_heap() const {
            auto heap_size = keys_.size();
            heap_.resize(heap_size);
            std::iota(std::begin(heap_), std::end(heap_), 0);
        }

        void heapify(size_type parent_node_index) const {
            // Move down from 'parent_node_index' and restore the
            // heap property (i.e. the parent is bigger/smaller
            // then both of its children).
            auto heap_size = static_cast<size_type>(heap_.size());
            auto left_child_index = 2 * parent_node_index + 1;
            auto right_child_index = left_child_index + 1;
            auto i = parent_node_index;
            if (left_child_index < heap_size && comparator_(key(left_child_index), key(parent_node_index)))
                // swap left child with its parent
                i = left_child_index;
            if (right_child_index < heap_size && comparator_(key(right_child_index), key(i)))
                // swap right child with its parent
                i = right_child_index;
            if (i != parent_node_index) {
                std::swap(heap_[parent_node_index], heap_[i]);
                heapify(i);
            }
        }

        void swim_up(size_type node_index) {
            // From node_index, restore the heap property by moving
            // upwards.
            size_type parent_node_index = (node_index - 1) / 2;

            // as long as the child node is bigger/smaller than its parent
            while (node_index > 0 && comparator_(key(node_index), key(parent_node_index)))
            {
                // swap child with parent
                std::swap(heap_[parent_node_index], heap_[node_index]);
                node_index = parent_node_index;
                parent_node_index = (node_index - 1) / 2;
            }
        }

    private:
        std::function<T (size_type)> key = [this](size_type node_index) {
            return keys_[heap_[node_index]];
        };

        mutable bool   heaped_;
        mutable Heap_t heap_;
        std::vector<T> keys_;
        COMPARATOR<T>  comparator_;
    };

} // namespace common_NS
