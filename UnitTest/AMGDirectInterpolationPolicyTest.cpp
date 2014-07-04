#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"
#include "UnitTest/MultigridTutorialExampleP144Test.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace MultigridTutorialExampleP144Test_NS;
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

TEST_F(AMGDirectInterpolationPolicyTest, MadeUpExample) {
    // The graph for this example is taken from
    // "Distance-Two Interpolation for Parallel Algebraic Multigrid",
    // H. De Sterck, R. Falgout, J. Nolting, U.M. Yang,
    // May 10, 2007
    // page 8, fig. 4.1
    // Note: The matrix values are completely made up and have no meaning!
    // Characteristics:
    // - all connections are strong
    // - there are strong F-F connections

    class StrengthPolicyMock : public IAMGStandardStrengthPolicy {
    public:
        StrengthPolicyMock() {
            variable_set_[0].add(1);

            variable_set_[1].add(0);
            variable_set_[1].add(5);
            variable_set_[1].add(2);
            variable_set_[1].add(8);

            variable_set_[2].add(1);

            variable_set_[3].add(4);

            variable_set_[4].add(3);
            variable_set_[4].add(5);
            variable_set_[4].add(8);
            variable_set_[4].add(9);

            variable_set_[5].add(7);
            variable_set_[5].add(6);
            variable_set_[5].add(1);
            variable_set_[5].add(4);

            variable_set_[6].add(5);

            variable_set_[7].add(5);

            variable_set_[8].add(1);
            variable_set_[8].add(4);

            variable_set_[9].add(4);
        }

        std::shared_ptr<IVariableSet> GetInfluencedByVariables(IMatrix2D::size_type variable) const override {
            return std::make_shared<VariableSet>(variable_set_[variable]);
        }

        std::shared_ptr<IVariableSet> GetDependentOnVariables(IMatrix2D::size_type variable) const override {
            return nullptr;
        }

    private:
        mutable std::map<IMatrix2D::size_type, VariableSet> variable_set_;
    };



    SparseMatrix2D m{10};

    m(0, 0) = 8;
    m(0, 1) = -5;

    m(1, 0) = -3;
    m(1, 1) = 13;
    m(1, 2) = -2;
    m(1, 5) = -1;
    m(1, 8) = -7;

    m(2, 1) = -1;
    m(2, 2) = 7;

    m(3, 3) = 12;
    m(3, 4) = -3;

    m(4, 3) = -7;
    m(4, 4) = 21;
    m(4, 5) = -5;
    m(4, 8) = -3;
    m(4, 9) = -2;

    m(5, 1) = -5;
    m(5, 4) = -1;
    m(5, 5) = 11;
    m(5, 6) = -3;
    m(5, 7) = -2;

    m(6, 6) = 5;
    m(6, 7) = -2;

    m(7, 5) = -2;
    m(7, 7) = 2;

    m(8, 1) = -3;
    m(8, 4) = -2;
    m(8, 8) = 5;

    m(9, 4) = -1;
    m(9, 9) = 2;

    m.finalize();

    // set up type of variables
    VariableCategorizer variable_categorizer{10};
    variable_categorizer.SetType(0, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(2, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(4, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(5, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(6, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(7, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(8, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(9, VariableCategorizer::Type::COARSE);


    StrengthPolicyMock strength_policy;
    AMGDirectInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(2.0 / 5.0, interpolation_operator(5, 1));
    ASSERT_DOUBLE_EQ(3.0 / 5.0, interpolation_operator(5, 0));

    ASSERT_DOUBLE_EQ(1.0, interpolation_operator(1, 2));
}

TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithWeakConnections) {
 
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1.0 / 2.0,         -2, -1.0 / 2.0,
                       -1, 29.0 / 4.0, -1,
               -1.0 / 8.0,         -2, -1.0 / 8.0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();


    AMGDirectInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, MultigridTutorialExampleP144Test::GetStrengthPolicy(), MultigridTutorialExampleP144Test::GetVariableCategorizer()));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(4, 0));
    ASSERT_DOUBLE_EQ(1.0 / 6.0, interpolation_operator(4, 1));
    ASSERT_DOUBLE_EQ(1.0 / 6.0, interpolation_operator(4, 2));
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(4, 3));
}

TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithPositiveOffDiagonalEntries) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1.0 / 2.0,         -2, -1.0 / 2.0,
                       -1, 29.0 / 4.0, -1,
                1.0 / 8.0,         -2,  1.0 / 8.0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();


    AMGDirectInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, MultigridTutorialExampleP144Test::GetStrengthPolicy(), MultigridTutorialExampleP144Test::GetVariableCategorizer()));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(14.0 / 45.0, interpolation_operator(4, 0));
    ASSERT_DOUBLE_EQ(7.0 / 45.0, interpolation_operator(4, 1));
    ASSERT_DOUBLE_EQ(7.0 / 45.0, interpolation_operator(4, 2));
    ASSERT_DOUBLE_EQ(14.0 / 45.0, interpolation_operator(4, 3));
}

TEST(AMGDirectInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithStrongPositiveOffDiagonalEntries) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1.0 / 2.0,          2, -1.0 / 2.0,
                       -1, 29.0 / 4.0, -1,
                1.0 / 8.0,          2,  1.0 / 8.0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();


    AMGDirectInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, MultigridTutorialExampleP144Test::GetStrengthPolicy(), MultigridTutorialExampleP144Test::GetVariableCategorizer()));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(- 17.0 / 58.0, interpolation_operator(4, 0));
    ASSERT_DOUBLE_EQ(6.0 / 29.0, interpolation_operator(4, 1));
    ASSERT_DOUBLE_EQ(6.0 / 29.0, interpolation_operator(4, 2));
    ASSERT_DOUBLE_EQ(- 17.0 / 58, interpolation_operator(4, 3));
}
