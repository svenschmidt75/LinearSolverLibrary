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
    BinaryHeap<double, std::greater> binary_heap;
    /*
     *           25
     *          /  \
     *         39  22
     */
    binary_heap.add(25);
    binary_heap.add(39);
    binary_heap.add(22);
    binary_heap.build_heap();

    /*
     *           39
     *          /  \
     *         25  22
     */
    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
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
    BinaryHeap<double, std::greater> binary_heap;
    /*
     *           25
     *          /  \
     *         22  39
     */
    binary_heap.add(25);
    binary_heap.add(22);
    binary_heap.add(39);
    binary_heap.build_heap();

    /*
     *           39
     *          /  \
     *         22  25
     */
    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
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
    BinaryHeap<double, std::greater> binary_heap;
    /*
     *           22
     *          /  \
     *         25  39
     */
    binary_heap.add(22);
    binary_heap.add(25);
    binary_heap.add(39);
    binary_heap.build_heap();

    /*
     *           39
     *          /  \
     *         25  22
     */
    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
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
    BinaryHeap<double, std::greater> binary_heap;
    binary_heap.add(22);
    binary_heap.build_heap();
}

namespace {

    struct NonTrivialType {
        double x;
    };

    template<typename T>
    struct Comparartor {
        bool operator()(struct NonTrivialType const & lhs, struct NonTrivialType const & rhs) const {
            return lhs.x > rhs.x;
        }
    };

}

void
BinaryHeapTest::TestBuildHeap5() {
    BinaryHeap<NonTrivialType, Comparartor> binary_heap;
    /*
     *           22
     *          /  \
     *         25  39
     */
    binary_heap.add({22});
    binary_heap.add({25});
    binary_heap.add({39});
    binary_heap.build_heap();

    /*
     *           39
     *          /  \
     *         25  22
     */
    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
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
    BinaryHeap<double, std::greater> binary_heap;
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
    binary_heap.add(13);
    binary_heap.add(14);
    binary_heap.add(15);
    binary_heap.add(18);
    binary_heap.add(11);
    binary_heap.add(12);
    binary_heap.add(17);
    binary_heap.add(16);
    binary_heap.build_heap();

    /*
     */
    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
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
    BinaryHeap<double, std::greater> binary_heap;
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
        binary_heap.add(item.second);
    binary_heap.build_heap();

    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
    };
    for (auto item : expected) {
        double exp = item.second;
        double value = get_value(item.first);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", exp, value, 1E-10);
    }
}

void
BinaryHeapTest::TestInsertSimple() {
    BinaryHeap<double, std::greater> binary_heap;
    /*
     *           25
     *          /  \
     *         22  39
     */
    binary_heap.add(25);
    binary_heap.add(22);
    binary_heap.insert(39);

    /*
     *           39
     *          /  \
     *         22  25
     */
    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
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
    BinaryHeap<double, std::greater> binary_heap;
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
        binary_heap.add(item.second);
    binary_heap.build_heap();

    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
    };

    binary_heap.insert(39);
    double exp = 39;
    double value = get_value(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("build heap error", exp, value, 1E-10);
}

void
BinaryHeapTest::TestRemoveThrowsWhenEmpty() {
    BinaryHeap<double, std::greater> binary_heap;
    CPPUNIT_ASSERT_THROW_MESSAGE("Exception not thrown", binary_heap.remove(1), std::runtime_error);
}

void
BinaryHeapTest::TestRemoveFromEnd() {
    BinaryHeap<double, std::greater> binary_heap;
    /*
     *           22
     *          /  \
     *         25  39
     */
    binary_heap.add(22);
    binary_heap.add(25);
    binary_heap.add(39);
    binary_heap.build_heap();

    /*
     *           39
     *          /  \
     *         25  22
     */
    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
    };

    binary_heap.remove(2);

    double expected = 39.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("heap error", expected, get_value(0), 1E-10);
    expected = 25.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("heap error", expected, get_value(1), 1E-10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("heap error", 2ull, binary_heap.keys_.size());
}

void
BinaryHeapTest::TestRemoveInside1() {
    BinaryHeap<double, std::greater> binary_heap;
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
        binary_heap.add(item.second);
    binary_heap.build_heap();

    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
    };

    // remove -55
    binary_heap.remove(6);

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
BinaryHeapTest::TestRemoveInside2() {
    BinaryHeap<double, std::greater> binary_heap;
    /*
     *                           39
     *                         /    \
     *                        /      \
     *                       /        \
     *                      /          \
     *                     33           3
     *                   /    \       /   \
     *                  17    22    -34   -55   
     *                 /  \ 
     *                4   14
     */
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
        binary_heap.add(item.second);
    binary_heap.build_heap();

    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
    };

    // remove item 1, 33
    binary_heap.remove(1);

    std::initializer_list<std::pair<int, double>> expected = {
        {0, 39.0},
        {1, 22.0},
        {2, 3.0},
        {3, 17.0},
        {4, 14.0},
        {5, -34.0},
        {6, -55.0},
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
    BinaryHeap<double, std::greater> binary_heap;
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
        binary_heap.add(item.second);
    binary_heap.build_heap();

    auto get_value = [&binary_heap](int node_index) {
        return binary_heap.keys_[binary_heap.heap_[node_index]];
    };

    // remove root, 45
    binary_heap.remove(0);

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

void
BinaryHeapTest::TestGetElement() {
    BinaryHeap<double, std::greater> binary_heap;
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
        {1, 3.0},
        {2, 39.0},
        {3, 17.0},
        {4, 22.0},
        {5, -34.0},
        {6, 4.0},
        {7, 14.0},
        {8, 33.0},
        {9, -55.0}
    };
    for (auto item : initial_structure)
        binary_heap.add(item.second);
    binary_heap.build_heap();

    double expected = 45.0;
    double value = binary_heap.getElement(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("heap error", expected, value, 1E-10);

    expected = -55.0;
    value = binary_heap.getElement(9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("heap error", expected, value, 1E-10);

    expected = 14.0;
    value = binary_heap.getElement(7);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("heap error", expected, value, 1E-10);
}

void
BinaryHeapTest::TestGetIndexForElement() {
    BinaryHeap<double, std::greater> binary_heap;
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
        {1, 3.0},
        {2, 39.0},
        {3, 17.0},
        {4, 22.0},
        {5, -34.0},
        {6, 4.0},
        {7, 14.0},
        {8, 33.0},
        {9, -55.0}
    };
    for (auto item : initial_structure)
        binary_heap.add(item.second);
    binary_heap.build_heap();

    decltype(binary_heap.getIndexForElement(0)) expected = 0;
    auto value = binary_heap.getIndexForElement(45.0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("heap error", expected, value);

    expected = 9;
    value = binary_heap.getIndexForElement(-55.0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("heap error", expected, value);

    expected = 7;
    value = binary_heap.getIndexForElement(14.0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("heap error", expected, value);
}

void
BinaryHeapTest::TestCheckHeapProperty() {
BinaryHeap<double, std::greater> binary_heap;
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
        {1, 3.0},
        {2, 39.0},
        {3, 17.0},
        {4, 22.0},
        {5, -34.0},
        {6, 4.0},
        {7, 14.0},
        {8, 33.0},
        {9, -55.0}
    };
    for (auto item : initial_structure)
        binary_heap.add(item.second);
    CPPUNIT_ASSERT_MESSAGE("heap error", binary_heap.check_heap_property() == false);
    binary_heap.build_heap();
    CPPUNIT_ASSERT_MESSAGE("heap error", binary_heap.check_heap_property());
}
