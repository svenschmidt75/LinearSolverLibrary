#include "pch.h"

#include "PriorityArrayTest.h"

#include "common/PriorityArray.h"


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
    class Comparartor {
    public:
        bool operator()(T const & lhs, T const & rhs) const {
            return lhs.priority > rhs.priority;
        }
    };
}

void
PriorityArrayTest::TestPop1() {
    PriorityArray<NonTrivialType, Comparartor> priority_array;
     /*
     *           25
     *          /  \
     *         39  22
     */
    priority_array.add(struct NonTrivialType{25});
    priority_array.add(struct NonTrivialType{39});
    priority_array.add(struct NonTrivialType{22});

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
        int index;
    };

    template<typename T>
    class Comparator2 {
    public:
        Comparator2(std::vector<int> & priorities) : priorities_{priorities} {}

        Comparator2(Comparator2 const & in) : priorities_{in.priorities_} {}

        bool operator()(T const & lhs, T const & rhs) const {
            return priorities_[lhs.index] > priorities_[rhs.index];
        }

        std::vector<int> & priorities_;
    };
}

void
PriorityArrayTest::TestUpdatePriority() {
    std::vector<int> priorities{25, 39, 22};

    PriorityArray<NonTrivialType2, Comparator2> priority_array{
        Comparator2<struct NonTrivialType2>{priorities}
    };
     /*
     *           25
     *          /  \
     *         39  22
     */
    priority_array.add(struct NonTrivialType2{0});
    priority_array.add(struct NonTrivialType2{1});
    priority_array.add(struct NonTrivialType2{2});

    /*
     *           39
     *          /  \
     *         25  22
     */
    CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 39, priorities[priority_array.top().index]);

    // change priority of element 25 in position 1 and update
    priorities[0] = 50;
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
        CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", expected, expected_priorities[element.index]);
        priority_array.pop();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("priority array error", 3ull - i, priority_array.heap_.keys_.size());
    }
}
