#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


class AMGDirectInterpolationPolicyTest : public Test {
public:
    void SetUp() override {
        MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
        stencil << 0, -1,  0,
                  -1,  4, -1,
                   0, -1,  0;
        SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);
        success = splitting_policy.Generate(m);
    }

public:
    AMGDirectInterpolationPolicy splitting_policy;
    bool                         success;
};


TEST_F(AMGDirectInterpolationPolicyTest, TestInitialization) {
    ASSERT_THAT(success, true);
}

TEST_F(AMGDirectInterpolationPolicyTest, TestInterpolationOperatorHasCorrectRowDimension) {
    auto const & interpolation_operator = splitting_policy.Interpolator();
    EXPECT_EQ(interpolation_operator.rows(), 9);
}

TEST_F(AMGDirectInterpolationPolicyTest, TestInterpolationOperatorHasCorrectColumnDimension) {
    auto const & interpolation_operator = splitting_policy.Interpolator();
    EXPECT_EQ(interpolation_operator.cols(), 5);
}

TEST_F(AMGDirectInterpolationPolicyTest, TestInterpolationOperator1stRow) {
    auto const & interpolation_operator = splitting_policy.Interpolator();
    std::vector<double> expected;
    for (int i{0}; i < interpolation_operator.cols(); ++i)
        expected.push_back(interpolation_operator(0, i));
    double tol = 1E-15;
    ASSERT_THAT(expected, ElementsAre(DoubleNear(1.0, tol), DoubleNear(0.0, tol), DoubleNear(0.0, tol), DoubleNear(0.0, tol), DoubleNear(0.0, tol)));
}

TEST_F(AMGDirectInterpolationPolicyTest, TestInterpolationOperator2ndRow) {
    auto const & interpolation_operator = splitting_policy.Interpolator();
    std::vector<double> expected;
    for (int i{0}; i < interpolation_operator.cols(); ++i)
        expected.push_back(interpolation_operator(1, i));
    double tol = 1E-15;
    ASSERT_THAT(expected, ElementsAre(DoubleNear(0.25, tol), DoubleNear(0.25, tol), DoubleNear(0.25, tol), DoubleNear(0.0, tol), DoubleNear(0.0, tol)));
}

TEST_F(AMGDirectInterpolationPolicyTest, TestInterpolationOperator6thdRow) {
    auto const & interpolation_operator = splitting_policy.Interpolator();
    std::vector<double> expected;
    for (int i{ 0 }; i < interpolation_operator.cols(); ++i)
        expected.push_back(interpolation_operator(5, i));
    double tol = 1E-15;
    ASSERT_THAT(expected, ElementsAre(DoubleNear(0.0, tol), DoubleNear(0.25, tol), DoubleNear(0.25, tol), DoubleNear(0.0, tol), DoubleNear(0.25, tol)));
}

TEST_F(AMGDirectInterpolationPolicyTest, TestProlongationOperatorHasCorrectRowDimension) {
    auto const & prolongation_operator = splitting_policy.Restrictor();
    EXPECT_EQ(prolongation_operator.rows(), 5);
}

TEST_F(AMGDirectInterpolationPolicyTest, TestProlongationOperatorHasCorrectColumnDimension) {
    auto const & prolongation_operator = splitting_policy.Restrictor();
    EXPECT_EQ(prolongation_operator.cols(), 9);
}

TEST_F(AMGDirectInterpolationPolicyTest, TestGalerkinMatrixHasCorrectRowDimension) {
    auto const & galerkin_operator = splitting_policy.GalerkinOperator();
    EXPECT_EQ(galerkin_operator.rows(), 5);
}

TEST_F(AMGDirectInterpolationPolicyTest, TestGalerkinMatrixHasCorrectColumnDimension) {
    auto const & galerkin_operator = splitting_policy.GalerkinOperator();
    EXPECT_EQ(galerkin_operator.cols(), 5);
}
