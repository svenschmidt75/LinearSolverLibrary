#include "pch.h"

#include "AMGWCycleTest.h"

#include "LinearSolverLibrary/AMGWCycle.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGWCycleTest::setUp() {}

void
AMGWCycleTest::tearDown() {}

void
AMGWCycleTest::TestThatThrowsOnZeroGridHierarchyDepth() {
    size_t grid_hierarchy_depth{0};
    CPPUNIT_ASSERT_THROW(AMGWCycle({2, grid_hierarchy_depth}), std::runtime_error);
}

void
AMGWCycleTest::Test1LevelGridHierarchy() {
    size_t grid_hierarchy_depth{1};
    AMGWCycle cycle{2, grid_hierarchy_depth};
    size_t expected{0};
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Level should be 0", expected, cycle.currentLevel());
    expected = grid_hierarchy_depth;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Level should be 1", expected, cycle.setNextLevel());
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Level should be 0", expected, cycle.setNextLevel());
}

void
AMGWCycleTest::Test2LevelGridHierarchy() {
    size_t grid_hierarchy_depth{2};
    AMGWCycle cycle{2, grid_hierarchy_depth};
    std::initializer_list<size_t> expected_levels{0, 1, 2, 1, 2, 1, 0};
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.currentLevel());
    }
}

void
AMGWCycleTest::Test3LevelGridHierarchy() {
    size_t grid_hierarchy_depth{3};
    AMGWCycle cycle{2, grid_hierarchy_depth};
    std::initializer_list<size_t> expected_levels{0, 1, 2, 3, 2, 3, 2, 1, 2, 3, 2, 3, 2, 1, 0};
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.currentLevel());
    }
}
