#include "pch.h"

#include "LinearSolverLibrary/AMGVCycle.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(AMGVCycleTest, TestThatThrowsOnZeroGridHierarchyDepth) {
    int grid_hierarchy_depth{0};
    AMGVCycle cycle;
    ASSERT_THROW(cycle.build(grid_hierarchy_depth), std::runtime_error);
}

TEST(AMGVCycleTest, Test3LevelGridHierarchyVCycle) {
    int grid_hierarchy_depth{3};
    AMGVCycle cycle;
    cycle.build(grid_hierarchy_depth);
    std::vector<int> expexted_levels{0, 1, 2, 3, 2, 1, 0};
    ASSERT_THAT(std::vector<int>(std::cbegin(cycle), std::cend(cycle)), ElementsAreArray(expexted_levels));
}
