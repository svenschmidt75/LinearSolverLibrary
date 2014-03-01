#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGHierarchyBuilder.hpp"
#include "LinearSolverLibrary/AMGMonitor.h"
#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


class AMGHierarchyBuilderTest : public Test {
public:
    void SetUp() override {
        MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
        stencil << 0, -1,  0,
                  -1,  4, -1,
                   0, -1,  0;
        SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

        AMGDirectInterpolationPolicy splitting_policy;
        AMGMonitor monitor;
        monitor.direct_solver_threshold_ = 3;
        AMGHierarchyBuilder<AMGDirectInterpolationPolicy> builder{monitor};
        amg_levels = builder.build(m);
    }

public:
    std::vector<AMGLevel> amg_levels;
};


TEST_F(AMGHierarchyBuilderTest, TestNumberOfLevels) {
    ASSERT_EQ(amg_levels.size(), 3);
}

TEST_F(AMGHierarchyBuilderTest, TestThat1stLevelHasNoInterpolationOperator) {
    ASSERT_EQ(amg_levels[0].interpolator.get(), nullptr);
}

TEST_F(AMGHierarchyBuilderTest, TestThatLastLevelHasNoRestrictionOperator) {
    ASSERT_EQ(amg_levels[2].restrictor.get(), nullptr);
}

TEST_F(AMGHierarchyBuilderTest, Test1stLevelRestrictionOperatorRowDimension) {
    ASSERT_EQ(amg_levels[0].restrictor->rows(), 5);
}

TEST_F(AMGHierarchyBuilderTest, Test1stLevelRestrictionOperatorColumnDimension) {
    ASSERT_EQ(amg_levels[0].restrictor->cols(), 9);
}

TEST_F(AMGHierarchyBuilderTest, Test2ndLevelRestrictionOperatorRowDimension) {
    ASSERT_EQ(amg_levels[1].restrictor->rows(), 1);
}

TEST_F(AMGHierarchyBuilderTest, Test2ndLevelRestrictionOperatorColumnDimension) {
    ASSERT_EQ(amg_levels[1].restrictor->cols(), 5);
}

TEST_F(AMGHierarchyBuilderTest, Test2ndLevelInterpolationOperatorRowDimension) {
    ASSERT_EQ(amg_levels[1].interpolator->rows(), 9);
}

TEST_F(AMGHierarchyBuilderTest, Test2ndLevelInterplationOperatorColumnDimension) {
    ASSERT_EQ(amg_levels[1].interpolator->cols(), 5);
}

TEST_F(AMGHierarchyBuilderTest, Test3rdLevelInterpolationOperatorRowDimension) {
    ASSERT_EQ(amg_levels[2].interpolator->rows(), 5);
}

TEST_F(AMGHierarchyBuilderTest, Test3rdLevelInterplationOperatorColumnDimension) {
    ASSERT_EQ(amg_levels[2].interpolator->cols(), 1);
}
