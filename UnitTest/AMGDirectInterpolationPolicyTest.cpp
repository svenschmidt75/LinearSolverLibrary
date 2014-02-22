#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"
#include "LinearSolverLibrary/AMGStandardSplitting.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


class DirectInterpolationPolicyTest : public Test {
public:
    void SetUp() override {
        MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
        stencil << 0, -1,  0,
                  -1,  4, -1,
                   0, -1,  0;
        SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);
        success = splitting_policy.generate(m);
    }

public:
    AMGDirectInterpolationPolicy splitting_policy;
    bool                         success;
};


TEST_F(DirectInterpolationPolicyTest, TestInitialization) {
    ASSERT_THAT(success, true);
}

TEST_F(DirectInterpolationPolicyTest, TestInterpolationOperatorHasCorrectRows) {
    auto const & interpolation_operator = splitting_policy.interpolator();
    EXPECT_EQ(interpolation_operator.rows(), 9);
}

TEST_F(DirectInterpolationPolicyTest, TestInterpolationOperatorHasCorrectCols) {
    auto const & interpolation_operator = splitting_policy.interpolator();
    EXPECT_EQ(interpolation_operator.cols(), 5);
}
