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
PriorityArrayTest::Test1() {
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
        CPPUNIT_ASSERT_EQUAL_MESSAGE("priority error", expected, element.priority);
        priority_array.pop();
    }
}
