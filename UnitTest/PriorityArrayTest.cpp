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
    class Comparer {
    public:
        bool operator()(T const & lhs, T const & rhs) const {
            return lhs.priority > rhs.priority;
        }
    };
}

void
PriorityArrayTest::TestPop1() {
    PriorityArray<NonTrivialType, Comparer> priority_array;
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

    template<typename T>
    class Comparer2 {
        Comparer2(int indices[]) : indices_{indices} {}

    public:
        bool operator()(T const & lhs, T const & rhs) const {
            return lhs.priority > rhs.priority;
        }

    private:
        int indices_[];
    };
}

void
PriorityArrayTest::TestUpdatePriority() {
    PriorityArray<NonTrivialType, Comparer2> priority_array{};
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


    priority_array.updatePriority(2);


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
