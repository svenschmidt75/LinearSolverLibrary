#include "pch.h"

#include "AMGVCycleTest.h"

#include "LinearSolverLibrary/AMGVCycle.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGVCycleTest::setUp() {}

void
AMGVCycleTest::tearDown() {}

void
AMGVCycleTest::TestThatThrowsOnZeroGridHierarchyDepth() {
    int grid_hierarchy_depth{0};
    AMGVCycle cycle;
    CPPUNIT_ASSERT_THROW(cycle.build(grid_hierarchy_depth), std::runtime_error);
}

void
AMGVCycleTest::Test3LevelGridHierarchyVCycle() {
    int grid_hierarchy_depth{3};
    AMGVCycle cycle;
    cycle.build(grid_hierarchy_depth);
    std::initializer_list<int> expected_levels{ 0, 1, 2, 3, 2, 1, 0 };
    for (auto expected_level : expected_levels) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Level unexpected", expected_level, cycle.currentLevel());
        cycle.setNextLevel();
    }
}
