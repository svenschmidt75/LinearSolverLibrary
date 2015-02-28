#include "pch.h"

#include "MultigridTutorialExampleTestFramework.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


class MultigridTutorialExampleTestFrameworkTest : public Test {
public:
    void SetUp() override {
        mesh_size = 5;
        framework = MultigridTutorialExampleTestFramework{mesh_size};
        framework.InitializeWithStencil1();
        tol_ = 1E-15;
    }

public:
    int                                   mesh_size;
    double                                tol_;
    MultigridTutorialExampleTestFramework framework;
};


TEST_F(MultigridTutorialExampleTestFrameworkTest, TestFactorForStencil1) {
    framework.InitializeWithStencil1();
    ASSERT_THAT(framework.Factor(), DoubleNear(1.0 / ((mesh_size + 1) * (mesh_size + 1)), tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestFactorForStencil2) {
    framework.InitializeWithStencil2();
    ASSERT_THAT(framework.Factor(), DoubleNear(2.0 / ((mesh_size + 1) * (mesh_size + 1)), tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestFactorForStencil3) {
    framework.InitializeWithStencil3();
    ASSERT_THAT(framework.Factor(), DoubleNear(3.0 / ((mesh_size + 1) * (mesh_size + 1)), tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestFactorForStencil4) {
    framework.InitializeWithStencil4();
    ASSERT_THAT(framework.Factor(), DoubleNear(6.0 / ((mesh_size + 1) * (mesh_size + 1)), tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestGetXCoordinateThrowsForLowerLimitViolation) {
    ASSERT_THROW(framework.GetX(-1), std::runtime_error);
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestGetXCoordinateThrowsForUpperLimitViolation) {
    ASSERT_THROW(framework.GetX(5), std::runtime_error);
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test1stGridPointXCoordinate) {
    double offset = 1.0 / (mesh_size + 1);
    ASSERT_THAT(framework.GetX(0), DoubleNear(offset, tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test2ndGridPointXCoordinate) {
    double offset = 1.0 / (mesh_size + 1);
    ASSERT_THAT(framework.GetX(1), DoubleNear(offset + 1.0 / (mesh_size + 1), tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test4thGridPointXCoordinate) {
    double offset = 1.0 / (mesh_size + 1);
    ASSERT_THAT(framework.GetX(4), DoubleNear(offset + 4.0 / (mesh_size + 1), tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestGetYCoordinateThrowsForLowerLimitViolation) {
    ASSERT_THROW(framework.GetY(-1), std::runtime_error);
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestGetYCoordinateThrowsForUpperLimitViolation) {
    ASSERT_THROW(framework.GetY(5), std::runtime_error);
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test1stGridPointYCoordinate) {
    double offset = 1.0 / (mesh_size + 1);
    ASSERT_THAT(framework.GetY(0), DoubleNear(offset, tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test2ndGridPointYCoordinate) {
    double offset = 1.0 / (mesh_size + 1);
    ASSERT_THAT(framework.GetY(1), DoubleNear(offset + 1.0 / (mesh_size + 1), tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test4thGridPointYCoordinate) {
    double offset = 1.0 / (mesh_size + 1);
    ASSERT_THAT(framework.GetY(4), DoubleNear(offset + 4.0 / (mesh_size + 1), tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestThatRightHandSideAtBoundaryIsZero) {
    ASSERT_THAT(framework.f(0, 0), DoubleNear(0.0, tol_));
    ASSERT_THAT(framework.f(0, 1), DoubleNear(0.0, tol_));
    ASSERT_THAT(framework.f(1, 1), DoubleNear(0.0, tol_));
    ASSERT_THAT(framework.f(1, 0), DoubleNear(0.0, tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestExactSolutionAtBoundaryIsZero) {
    ASSERT_THAT(framework.Solution(0, 0), DoubleNear(0.0, tol_));
    ASSERT_THAT(framework.Solution(0, 1), DoubleNear(0.0, tol_));
    ASSERT_THAT(framework.Solution(1, 1), DoubleNear(0.0, tol_));
    ASSERT_THAT(framework.Solution(1, 0), DoubleNear(0.0, tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestExactSolutionForInnerNodes) {
    double x = framework.GetX(3);
    double y = framework.GetY(2);
    ASSERT_THAT(framework.Solution(x, y), DoubleNear(-0.046296296296296294, tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestDirectSolveIsConsistent) {
    Vector solution = framework.DirectSolve();
    Vector ax = framework.m_ * solution;
    Vector rhs = framework.CreateRHS();
    double error = VectorMath::LinfError(ax, rhs);
    ASSERT_THAT(error, DoubleNear(0.0, tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestDirectSolveL2Error) {
    Vector solution = framework.DirectSolve();
    Vector ax = framework.m_ * solution;
    Vector rhs = framework.CreateRHS();
    double error = VectorMath::L2Error(ax, rhs);
    ASSERT_THAT(error, DoubleNear(0.0, tol_));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestDirectSolve) {
    // Note: The result is not exact as the mesh spacing is rather coarse,
    // but we compare against the exact solution.
    // Decreasing the mesh size will show that the error is gradually
    // reduced though.
    Vector solution = framework.DirectSolve();
    double tol = 1E-5;
    ASSERT_THAT(framework.LinfError(solution), DoubleNear(0.0013471485790158694, tol));
}
