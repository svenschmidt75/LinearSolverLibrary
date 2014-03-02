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
        tol = 1E-15;
    }

public:
    int                                   mesh_size;
    double                                tol;
    MultigridTutorialExampleTestFramework framework;
};


TEST_F(MultigridTutorialExampleTestFrameworkTest, TestFactorForStencil1) {
    ASSERT_THAT(framework.Factor(), DoubleNear(1.0 / (mesh_size * mesh_size), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestFactorForStencil2) {
    framework.InitializeWithStencil2();
    ASSERT_THAT(framework.Factor(), DoubleNear(1.0 / (2.0 * (mesh_size * mesh_size)), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestFactorForStencil3) {
    framework.InitializeWithStencil3();
    ASSERT_THAT(framework.Factor(), DoubleNear(1.0 / (8.0 * mesh_size * mesh_size), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestFactorForStencil4) {
    framework.InitializeWithStencil4();
    ASSERT_THAT(framework.Factor(), DoubleNear(1.0 / (20.0 * mesh_size * mesh_size), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestGetXCoordinateThrowsForLowerLimitViolation) {
    ASSERT_THROW(framework.GetX(-1), std::runtime_error);
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestGetXCoordinateThrowsForUpperLimitViolation) {
    ASSERT_THROW(framework.GetX(5), std::runtime_error);
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test1stGridPointXCoordinate) {
    ASSERT_THAT(framework.GetX(0), DoubleNear(1.0 / (mesh_size + 1), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test2ndGridPointXCoordinate) {
    ASSERT_THAT(framework.GetX(1), DoubleNear(2.0 / (mesh_size + 1), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test4thGridPointXCoordinate) {
    ASSERT_THAT(framework.GetX(4), DoubleNear(5.0 / (mesh_size + 1), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestGetYCoordinateThrowsForLowerLimitViolation) {
    ASSERT_THROW(framework.GetY(-1), std::runtime_error);
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestGetYCoordinateThrowsForUpperLimitViolation) {
    ASSERT_THROW(framework.GetY(5), std::runtime_error);
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test1stGridPointYCoordinate) {
    ASSERT_THAT(framework.GetY(0), DoubleNear(1.0 / (mesh_size + 1), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test2ndGridPointYCoordinate) {
    ASSERT_THAT(framework.GetY(1), DoubleNear(2.0 / (mesh_size + 1), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, Test4thGridPointYCoordinate) {
    ASSERT_THAT(framework.GetY(4), DoubleNear(5.0 / (mesh_size + 1), tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestThatRightHandSideAtBoundaryIsZero) {
    ASSERT_THAT(framework.f(0, 0), DoubleNear(0.0, tol));
    ASSERT_THAT(framework.f(0, 1), DoubleNear(0.0, tol));
    ASSERT_THAT(framework.f(1, 1), DoubleNear(0.0, tol));
    ASSERT_THAT(framework.f(1, 0), DoubleNear(0.0, tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestExactSolutionAtBoundaryIsZero) {
    ASSERT_THAT(framework.Solution(0, 0), DoubleNear(0.0, tol));
    ASSERT_THAT(framework.Solution(0, 1), DoubleNear(0.0, tol));
    ASSERT_THAT(framework.Solution(1, 1), DoubleNear(0.0, tol));
    ASSERT_THAT(framework.Solution(1, 0), DoubleNear(0.0, tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestExactSolutionForInnerNodes) {
    double x = framework.GetX(3);
    double y = framework.GetY(2);
    ASSERT_THAT(framework.Solution(x, y), DoubleNear(-0.0462962962962963, tol));
}

TEST_F(MultigridTutorialExampleTestFrameworkTest, TestDirectSolve) {
    Vector solution = framework.DirectSolve();
    ASSERT_THAT(framework.L1Error(solution), DoubleNear(1.0, tol));
}
