#include "pch.h"

#include "PriorityArrayTest.h"

#include "common/PriorityArray.hpp"


using namespace common_NS;


void
PriorityArrayTest::setUp() {}

void
PriorityArrayTest::tearDown() {}

namespace {

    struct NonTrivialType {
        int priority;
    };

    template<typename T>
    class Comparator {
    public:
        bool operator()(T const & lhs, T const & rhs) const {
            return lhs.priority > rhs.priority;
        }
    };
}

void
PriorityArrayTest::TestPop1() {
    PriorityArray<NonTrivialType, Comparator> priority_array;
     /*
     *           25
     *          /  \
     *         39  22
     */
    priority_array.add(NonTrivialType{25});
    priority_array.add(NonTrivialType{39});
    priority_array.add(NonTrivialType{22});

    /*
     *           39
     *          /  \
     *         25  22
     */

    int priorities[] = {39, 25, 22};
    int i = 0;
    while (priority_array.empty() == false) {
        struct NonTrivialType element = priority_array.top();
        int expected = priorities[i++];
        CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", expected, element.priority);
        priority_array.pop();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 3ull - i, priority_array.heap_.keys_.size());
    }
}

namespace {

    struct NonTrivialType2 {
        int node_index;
    };

    template<typename T>
    class Comparator2 {
    public:
        using size_type = std::int64_t;

    public:
        Comparator2(std::map<size_type, int> & nodes) : nodes_{nodes} {}

        Comparator2(Comparator2 const & in) : nodes_{in.nodes_} {}

        bool operator()(T const & lhs, T const & rhs) const {
            return nodes_[lhs.node_index] > nodes_[rhs.node_index];
        }

        int getPriority(size_type node_index) const {
            return nodes_[node_index];
        }

        void setPriority(size_type node_index, int priority) {
            nodes_[node_index] = priority;
        }

        std::map<size_type, int> & nodes_;
    };
}

void
PriorityArrayTest::TestUpdatePriority() {
    std::map<std::int64_t, int> nodes = {
        {0, 25},
        {1, 39},
        {2, 22}
    };

    Comparator2<struct NonTrivialType2> comparator{nodes};
    PriorityArray<NonTrivialType2, Comparator2> priority_array{comparator};
     /*
     *           25
     *          /  \
     *         39  22
     */
    priority_array.add(NonTrivialType2{0});
    priority_array.add(NonTrivialType2{1});
    priority_array.add(NonTrivialType2{2});

    /*
     *           39
     *          /  \
     *         25  22
     */
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 39, comparator.getPriority(priority_array.top().node_index));

    // change priority of element 25 in position 1 and update
    comparator.setPriority(0, 50);
    priority_array.updatePriority(1);

     /*
      *           50
      *          /  \
      *         39  22
      */
    
    int expected_priorities[] = {50, 39, 22};
    int i = 0;
    while (priority_array.empty() == false) {
        struct NonTrivialType2 element = priority_array.top();
        int expected = expected_priorities[i++];
        CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", expected, expected_priorities[element.node_index]);
        priority_array.pop();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 3ull - i, priority_array.heap_.keys_.size());
    }
}

void
PriorityArrayTest::TestUpdatePrioritiesInDifferentOrders() {
    /* Here we change several priorities before updating any elements.
     * When updating one element, the priority of others have already
     * changed, but the elements have not yet been updated.
     * Check that after updating all of the changed elements, the
     * priority queue is consistent.
     */
    std::map<std::int64_t, int> nodes = {
        {0, 25},
        {1, 39},
        {2, 22}
    };

    Comparator2<struct NonTrivialType2> comparator{ nodes };
    PriorityArray<NonTrivialType2, Comparator2> priority_array{ comparator };
    /*
     *           25
     *          /  \
     *         39  22
     */

    // insert node 0 with priority 25
    priority_array.add(NonTrivialType2{0});

    // insert node 1 with priority 39
    priority_array.add(NonTrivialType2{1});

    // insert node 2 with priority 22
    priority_array.add(NonTrivialType2{2});

    /*
     *          1: 39
     *         /     \
     *      0: 25   2: 22
     */
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 39, comparator.getPriority(priority_array.top().node_index));

    // change priority of element 25 in position 1
    comparator.setPriority(0, 39);

    // change priority of element 39 in position 0
    comparator.setPriority(1, 12);

    // change priority of element 22 in position 2
    comparator.setPriority(2, 99);

    priority_array.updatePriority(1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 39, comparator.getPriority(priority_array.heap_.heap_[0]));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 12, comparator.getPriority(priority_array.heap_.heap_[1]));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 99, comparator.getPriority(priority_array.heap_.heap_[2]));

    priority_array.updatePriority(0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 99, comparator.getPriority(priority_array.heap_.heap_[0]));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 12, comparator.getPriority(priority_array.heap_.heap_[1]));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 39, comparator.getPriority(priority_array.heap_.heap_[2]));

    priority_array.updatePriority(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 99, comparator.getPriority(priority_array.heap_.heap_[0]));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 12, comparator.getPriority(priority_array.heap_.heap_[1]));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 39, comparator.getPriority(priority_array.heap_.heap_[2]));

     /*
      *           99
      *          /  \
      *         12  39
      */
}
