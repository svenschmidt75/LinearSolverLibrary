#include "pch.h"

#include "BinaryHeapTest.h"

#include "common/BinaryHeap.hpp"


using namespace common_NS;


void
BinaryHeapTest::setUp() {}

void
BinaryHeapTest::tearDown() {}

void
BinaryHeapTest::TestBuildHeap1() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *           25
     *          /  \
     *         39  22
     */
    priority_array.add(25);
    priority_array.add(39);
    priority_array.add(22);
    priority_array.build_heap();

    /*
     *           39
     *          /  \
     *         25  22
     */
    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };
    double expected = 39.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(0), 1E-10);
    expected = 25.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(1), 1E-10);
    expected = 22.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(2), 1E-10);
}

void
BinaryHeapTest::TestBuildHeap2() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *           25
     *          /  \
     *         22  39
     */
    priority_array.add(25);
    priority_array.add(22);
    priority_array.add(39);
    priority_array.build_heap();

    /*
     *           39
     *          /  \
     *         22  25
     */
    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };
    double expected = 39.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(0), 1E-10);
    expected = 22.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(1), 1E-10);
    expected = 25.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(2), 1E-10);
}

void
BinaryHeapTest::TestBuildHeap3() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *           22
     *          /  \
     *         25  39
     */
    priority_array.add(22);
    priority_array.add(25);
    priority_array.add(39);
    priority_array.build_heap();

    /*
     *           39
     *          /  \
     *         25  22
     */
    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };
    double expected = 39.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(0), 1E-10);
    expected = 25.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(1), 1E-10);
    expected = 22.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(2), 1E-10);
}

void
BinaryHeapTest::TestBuildHeap4() {
    BinaryHeap<double, std::greater> priority_array;
    priority_array.add(22);
    priority_array.build_heap();
}

namespace {

    struct NonTrivialType {
        double x;
    };

    template<typename T>
    struct Comparer {
        bool operator()(struct NonTrivialType const & lhs, struct NonTrivialType const & rhs) const {
            return lhs.x > rhs.x;
        }
    };

}

void
BinaryHeapTest::TestBuildHeap5() {
    BinaryHeap<NonTrivialType, Comparer> priority_array;
    /*
     *           22
     *          /  \
     *         25  39
     */
    priority_array.add({22});
    priority_array.add({25});
    priority_array.add({39});
    priority_array.build_heap();

    /*
     *           39
     *          /  \
     *         25  22
     */
    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };
    double expected = 39.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(0).x, 1E-10);
    expected = 25.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(1).x, 1E-10);
    expected = 22.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(2).x, 1E-10);
}

void
BinaryHeapTest::TestBuildHeapComplex1() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *                           13
     *                          /  \
     *                         /    \
     *                        /      \
     *                       /        \
     *                      14        15
     *                     /  \      /  \
     *                    18   11   12  17
     *                   /
     *                  16
     */
    priority_array.add(13);
    priority_array.add(14);
    priority_array.add(15);
    priority_array.add(18);
    priority_array.add(11);
    priority_array.add(12);
    priority_array.add(17);
    priority_array.add(16);
    priority_array.build_heap();

    /*
     */
    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };
    std::initializer_list<std::pair<int, double>> expected = {
        {0, 18.0},
        {1, 16.0},
        {2, 17.0},
        {3, 14.0},
        {4, 11.0},
        {5, 12.0},
        {6, 15.0},
        {7, 13.0}
    };
    for (auto item : expected) {
        double exp = item.second;
        double value = get_value(item.first);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", exp, value, 1E-10);
    }
}

void
BinaryHeapTest::TestBuildHeapComplex2() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *                           45
     *                         /    \
     *                        /      \
     *                       /        \
     *                      /          \
     *                     33           3
     *                   /    \       /   \
     *                  17    25    -34   -55   
     *                 /  \   / \
     *                4   14 22
     */
    std::initializer_list<std::pair<int, double>> expected = {
        {0, 45.0},
        {1, 33.0},
        {2, 3.0},
        {3, 17.0},
        {4, 25.0},
        {5, -34.0},
        {6, -55.0},
        {7, 4.0},
        {8, 14.0},
        {9, 22.0}
    };
    for (auto item : expected)
        priority_array.add(item.second);
    priority_array.build_heap();

    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };
    for (auto item : expected) {
        double exp = item.second;
        double value = get_value(item.first);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", exp, value, 1E-10);
    }
}

void
BinaryHeapTest::TestInsertSimple() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *           25
     *          /  \
     *         22  39
     */
    priority_array.add(25);
    priority_array.add(22);
    priority_array.insert(39);

    /*
     *           39
     *          /  \
     *         22  25
     */
    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };
    double expected = 39.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(0), 1E-10);
    expected = 22.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(1), 1E-10);
    expected = 25.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", expected, get_value(2), 1E-10);
}

void
BinaryHeapTest::TestInsertComplex() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *                           45
     *                         /    \
     *                        /      \
     *                       /        \
     *                      /          \
     *                     33           3
     *                   /    \       /   \
     *                  /      \    -34   -55   
     *                 17      25
     *                /  \    /  \
     *               4   14  22
     */
    std::initializer_list<std::pair<int, double>> expected = {
        {0, 45.0},
        {1, 33.0},
        {2, 3.0},
        {3, 17.0},
        {4, 25.0},
        {5, -34.0},
        {6, -55.0},
        {7, 4.0},
        {8, 14.0},
        {9, 22.0}
    };
    for (auto item : expected)
        priority_array.add(item.second);
    priority_array.build_heap();

    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };

    priority_array.insert(39);
    double exp = 39;
    double value = get_value(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", exp, value, 1E-10);
}

void
BinaryHeapTest::TestRemoveThrowsWhenEmpty() {
    BinaryHeap<double, std::greater> priority_array;
    CPPUNIT_ASSERT_THROW_MESSAGE("Exception not thrown", priority_array.remove(1), std::runtime_error);
}

void
BinaryHeapTest::TestRemoveFromEnd() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *           22
     *          /  \
     *         25  39
     */
    priority_array.add(22);
    priority_array.add(25);
    priority_array.add(39);
    priority_array.build_heap();

    /*
     *           39
     *          /  \
     *         25  22
     */
    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };

    priority_array.remove(2);

    double expected = 39.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("heap error", expected, get_value(0), 1E-10);
    expected = 25.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("heap error", expected, get_value(1), 1E-10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("heap error", 2ull, priority_array.keys_.size());
}

void
BinaryHeapTest::TestRemoveInside() {
    BinaryHeap<double, std::greater> priority_array;
     std::initializer_list<std::pair<int, double>> initial_structure = {
        {0, 39.0},
        {1, 33.0},
        {2, 3.0},
        {3, 17.0},
        {4, 22.0},
        {5, -34.0},
        {6, -55.0},
        {7, 4.0},
        {8, 14.0}
    };
    for (auto item : initial_structure)
        priority_array.add(item.second);
    priority_array.build_heap();

    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };

    // remove -55
    priority_array.remove(6);

    std::initializer_list<std::pair<int, double>> expected = {
        {0, 39.0},
        {1, 33.0},
        {2, 14.0},
        {3, 17.0},
        {4, 22.0},
        {5, -34.0},
        {6, 3.0},
        {7, 4.0}
    };

    for (auto item : expected) {
        double exp = item.second;
        double value = get_value(item.first);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", exp, value, 1E-10);
    }
}

void
BinaryHeapTest::TestRemoveRoot() {
    BinaryHeap<double, std::greater> priority_array;
    /*
     *                           45
     *                         /    \
     *                        /      \
     *                       /        \
     *                      /          \
     *                     39           3
     *                   /    \       /   \
     *                  17    33    -34   -55   
     *                 /  \   / \
     *                4   14 22
     */
    std::initializer_list<std::pair<int, double>> initial_structure = {
        {0, 45.0},
        {1, 39.0},
        {2, 3.0},
        {3, 17.0},
        {4, 33.0},
        {5, -34.0},
        {6, -55.0},
        {7, 4.0},
        {8, 14.0},
        {9, 22.0}
    };
    for (auto item : initial_structure)
        priority_array.add(item.second);
    priority_array.build_heap();

    auto get_value = [&priority_array](int node_index) {
        return priority_array.keys_[priority_array.heap_[node_index]];
    };

    // remove root, 45
    priority_array.remove(0);

    std::initializer_list<std::pair<int, double>> expected = {
        {0, 39.0},
        {1, 33.0},
        {2, 3.0},
        {3, 17.0},
        {4, 22.0},
        {5, -34.0},
        {6, -55.0},
        {7, 4.0},
        {8, 14.0}
    };

    for (auto item : expected) {
        double exp = item.second;
        double value = get_value(item.first);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", exp, value, 1E-10);
    }
}
