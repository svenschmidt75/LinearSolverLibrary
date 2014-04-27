#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


class AMGHierarchyBuilderTest : public Test {
public:
    using size_type = IMatrix2D::size_type;

public:
    void SetUp() override {
        MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
        stencil << 0, -1,  0,
                  -1,  4, -1,
                   0, -1,  0;
        SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

        AMGDirectInterpolationPolicy splitting_policy;
        AMGMonitor monitor;
        monitor.direct_solver_threshold = 3;
        AMGHierarchyBuilder<AMGDirectInterpolationPolicy, AMGCThenFRelaxationPolicy> builder{monitor};
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

TEST_F(AMGHierarchyBuilderTest, TestCAndFVariableDecompositionFor1stLevel) {
    auto variable_decomposition = amg_levels[0].variableDecomposition;

    // check coarse variables
    auto variables = std::find_if(std::cbegin(variable_decomposition), std::cend(variable_decomposition), [](std::pair<size_type, std::set<size_type>> const & item) -> bool {
        return item.first == 0;
    });
    ASSERT_THAT(variables, Ne(std::cend(variable_decomposition)));
    ASSERT_THAT(variables->second, ElementsAre(0, 2, 4, 6, 8));

    // check fine variables
    variables = std::find_if(std::cbegin(variable_decomposition), std::cend(variable_decomposition), [](std::pair<size_type, std::set<size_type>> const & item) -> bool {
        return item.first == 1;
    });
    ASSERT_THAT(variables, Ne(std::cend(variable_decomposition)));
    ASSERT_THAT(variables->second, ElementsAre(1, 3, 5, 7));
}

TEST_F(AMGHierarchyBuilderTest, TestCAndFVariableDecompositionFor2ndLevel) {
    auto variable_decomposition = amg_levels[1].variableDecomposition;

    // check coarse variables
    auto variables = std::find_if(std::cbegin(variable_decomposition), std::cend(variable_decomposition), [](std::pair<size_type, std::set<size_type>> const & item) -> bool {
        return item.first == 0;
    });
    ASSERT_THAT(variables, Ne(std::cend(variable_decomposition)));
    ASSERT_THAT(variables->second, ElementsAre(2));

    // check fine variables
    variables = std::find_if(std::cbegin(variable_decomposition), std::cend(variable_decomposition), [](std::pair<size_type, std::set<size_type>> const & item) -> bool {
        return item.first == 1;
    });
    ASSERT_THAT(variables, Ne(std::cend(variable_decomposition)));
    ASSERT_THAT(variables->second, ElementsAre(0, 1, 3, 4));
}

TEST_F(AMGHierarchyBuilderTest, TestCAndFVariableDecompositionForLastLevel) {
    auto variable_decomposition = amg_levels[2].variableDecomposition;
    ASSERT_THAT(std::cbegin(variable_decomposition), Eq(std::cend(variable_decomposition)));
}

TEST_F(AMGHierarchyBuilderTest, TestNumberOfColumnsLessThanDirectSolveThreshold) {
        MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
        stencil << 0, -1,  0,
                  -1,  4, -1,
                   0, -1,  0;
    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    AMGDirectInterpolationPolicy splitting_policy;
    AMGMonitor monitor;
    monitor.direct_solver_threshold = 10;
    AMGHierarchyBuilder<AMGDirectInterpolationPolicy, AMGCThenFRelaxationPolicy> builder{monitor};
    ASSERT_THROW(builder.build(m), std::logic_error);
}
