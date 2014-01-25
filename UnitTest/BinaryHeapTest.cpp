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

void
BinaryHeapTest::TestBuildHeapComplex() {
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
