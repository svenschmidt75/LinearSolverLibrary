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
    int grid_hierarchy_depth{0};
    CPPUNIT_ASSERT_THROW(AMGWCycle({2, grid_hierarchy_depth}), std::runtime_error);
}

void
AMGWCycleTest::Test1LevelGridHierarchyVCycle() {
    int grid_hierarchy_depth{1};
    AMGWCycle cycle{2, grid_hierarchy_depth};
    int expected{0};
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Level should be 0", expected, cycle.setNextLevel());
    expected = grid_hierarchy_depth;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Level should be 1", expected, cycle.setNextLevel());
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Level should be 0", expected, cycle.setNextLevel());
}

void
AMGWCycleTest::Test3LevelGridHierarchyVCycle() {
    int grid_hierarchy_depth{3};
    AMGWCycle cycle{1, grid_hierarchy_depth};
    std::initializer_list<int> expected_levels{0, 1, 2, 3, 2, 1, 0};
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.setNextLevel());
    }
}

void
AMGWCycleTest::Test2LevelGridHierarchyGamma2() {
    int grid_hierarchy_depth{2};
    AMGWCycle cycle{2, grid_hierarchy_depth};
    std::initializer_list<int> expected_levels{0, 1, 2, 1, 2, 1, 0};
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.setNextLevel());
    }
}

void
AMGWCycleTest::Test2LevelGridHierarchyGamma3() {
    int grid_hierarchy_depth{2};
    AMGWCycle cycle{3, grid_hierarchy_depth};
    std::initializer_list<int> expected_levels{0, 1, 2, 1, 2, 1, 2, 1, 0};
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.setNextLevel());
    }
}

void
AMGWCycleTest::Test3LevelGridHierarchyGamma2() {
    int grid_hierarchy_depth{3};
    AMGWCycle cycle{2, grid_hierarchy_depth};
    std::initializer_list<int> expected_levels{0, 1, 2, 3, 2, 3, 2, 1, 2, 3, 2, 3, 2, 1, 0};
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.setNextLevel());
    }
}

void
AMGWCycleTest::Test3LevelGridHierarchyGamma3() {
    int grid_hierarchy_depth{3};
    AMGWCycle cycle{3, grid_hierarchy_depth};
    std::initializer_list<int> expected_levels{0, 1, 2, 3, 2, 3, 2, 3, 2, 1, 2, 3, 2, 3, 2, 3, 2, 1, 2, 3, 2, 3, 2, 3, 2, 1, 0 };
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.setNextLevel());
    }
}

void
AMGWCycleTest::Test4LevelGridHierarchyGamma2() {
    int grid_hierarchy_depth{4};
    AMGWCycle cycle{2, grid_hierarchy_depth};
    std::initializer_list<int> expected_levels{0, 1, 2, 3, 4, 3, 4, 3, 2, 3, 4, 3, 4, 3, 2, 1, 2, 3, 4, 3, 4, 3, 2, 3, 4, 3, 4, 3, 2, 1, 0};
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.setNextLevel());
    }
}
