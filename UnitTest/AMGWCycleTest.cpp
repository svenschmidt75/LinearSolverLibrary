#include "pch.h"

#include "LinearSolverLibrary/AMGWCycle.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(AMGWCycleTest, TestThatThrowsOnZeroGridHierarchyDepth) {
    int grid_hierarchy_depth{0};
    AMGWCycle cycle{2};
    ASSERT_THROW(cycle.build(grid_hierarchy_depth), std::runtime_error);
}

TEST(AMGWCycleTest, Test1LevelGridHierarchyVCycle) {
    int grid_hierarchy_depth{1};
    AMGWCycle cycle{2};
    cycle.build(grid_hierarchy_depth);
    ASSERT_THAT(std::vector<int>(std::cbegin(cycle), std::cend(cycle)), ElementsAre(0, 1, 0));
}

TEST(AMGWCycleTest, Test3LevelGridHierarchyVCycle) {
    int grid_hierarchy_depth{3};
    AMGWCycle cycle{1};
    cycle.build(grid_hierarchy_depth);
    ASSERT_THAT(std::vector<int>(std::cbegin(cycle), std::cend(cycle)), ElementsAre(0, 1, 2, 3, 2, 1, 0));
}

TEST(AMGWCycleTest, Test2LevelGridHierarchyGamma2) {
    int grid_hierarchy_depth{2};
    AMGWCycle cycle{2};
    cycle.build(grid_hierarchy_depth);
    ASSERT_THAT(std::vector<int>(std::cbegin(cycle), std::cend(cycle)), ElementsAre(0, 1, 2, 1, 2, 1, 0));
}

TEST(AMGWCycleTest, Test2LevelGridHierarchyGamma3) {
    int grid_hierarchy_depth{2};
    AMGWCycle cycle{3};
    cycle.build(grid_hierarchy_depth);
    ASSERT_THAT(std::vector<int>(std::cbegin(cycle), std::cend(cycle)), ElementsAre(0, 1, 2, 1, 2, 1, 2, 1, 0));
}

TEST(AMGWCycleTest, Test3LevelGridHierarchyGamma2) {
    int grid_hierarchy_depth{3};
    AMGWCycle cycle{2};
    cycle.build(grid_hierarchy_depth);
    std::vector<int> expexted_levels{0, 1, 2, 3, 2, 3, 2, 1, 2, 3, 2, 3, 2, 1, 0};
    ASSERT_THAT(std::vector<int>(std::cbegin(cycle), std::cend(cycle)), ElementsAreArray(expexted_levels));
}

TEST(AMGWCycleTest, Test3LevelGridHierarchyGamma3) {
    int grid_hierarchy_depth{3};
    AMGWCycle cycle{3};
    cycle.build(grid_hierarchy_depth);
    std::vector<int> expexted_levels{0, 1, 2, 3, 2, 3, 2, 3, 2, 1, 2, 3, 2, 3, 2, 3, 2, 1, 2, 3, 2, 3, 2, 3, 2, 1, 0};
    ASSERT_THAT(std::vector<int>(std::cbegin(cycle), std::cend(cycle)), ElementsAreArray(expexted_levels));
}

TEST(AMGWCycleTest, Test4LevelGridHierarchyGamma2) {
    int grid_hierarchy_depth{4};
    AMGWCycle cycle{2};
    cycle.build(grid_hierarchy_depth);
    std::vector<int> expexted_levels{0, 1, 2, 3, 4, 3, 4, 3, 2, 3, 4, 3, 4, 3, 2, 1, 2, 3, 4, 3, 4, 3, 2, 3, 4, 3, 4, 3, 2, 1, 0};
    ASSERT_THAT(std::vector<int>(std::cbegin(cycle), std::cend(cycle)), ElementsAreArray(expexted_levels));
}
