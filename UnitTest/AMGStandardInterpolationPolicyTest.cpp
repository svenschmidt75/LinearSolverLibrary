#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGStandardInterpolationPolicy.h"
#include "LinearSolverLibrary/VariableCategorizer.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnection) {
    // Example taken from "Distance-Two Interpolation for Parallel Algebraic Multigrid",
    // H. De Sterck, R. Falgout, J. Nolting, U.M. Yang,
    // May 10, 2007
    // page 7 and 9, eq (4.7)
    // Matrix based on fig. 4.2, page 8
    SparseMatrix2D m{4};

    m(0, 0) = 2;
    m(0, 1) = -1;

    m(1, 0) = -1;
    m(1, 1) = 2;
    m(1, 2) = -1;

    m(2, 1) = -1;
    m(2, 2) = 2;
    m(2, 3) = -1;

    m(3, 2) = -1;
    m(3, 3) = 2;

    m.finalize();

    VariableCategorizer variable_categorizer{4};
    variable_categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(2, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::COARSE);

    AMGStandardStrengthPolicy strength_policy{m};

    AMGStandardInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(2.0 / 3.0, interpolation_operator(1, 0));
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(1, 1));
}

TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithMock) {
    // same test as above, but with mocked strength policy
    class StrengthPolicyMock : public IAMGStandardStrengthPolicy {
    public:
        StrengthPolicyMock() {
            variable_set_[0].add(1);
            
            variable_set_[1].add(0);
            variable_set_[1].add(2);

            variable_set_[2].add(1);
            variable_set_[2].add(3);

            variable_set_[3].add(2);
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



    SparseMatrix2D m{4};

    m(0, 0) = 2;
    m(0, 1) = -1;

    m(1, 0) = -1;
    m(1, 1) = 2;
    m(1, 2) = -1;

    m(2, 1) = -1;
    m(2, 2) = 2;
    m(2, 3) = -1;

    m(3, 2) = -1;
    m(3, 3) = 2;

    m.finalize();

    // set up type of variables
    VariableCategorizer variable_categorizer{4};
    variable_categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(2, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::COARSE);


    StrengthPolicyMock strength_policy;
    AMGStandardInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(2.0 / 3.0, interpolation_operator(1, 0));
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(1, 1));
}

TEST(AMGStandardInterpolationPolicyTest, TestComplexGraph) {
    class StrengthPolicyMock : public IAMGStandardStrengthPolicy {
    public:
        StrengthPolicyMock() {
            variable_set_[0].add(2);
            variable_set_[0].add(4);
            variable_set_[0].add(5);
            variable_set_[0].add(7);
            variable_set_[0].add(8);

            variable_set_[1].add(2);

            variable_set_[2].add(1);
            variable_set_[2].add(0);

            variable_set_[3].add(4);

            variable_set_[4].add(0);
            variable_set_[4].add(3);

            variable_set_[5].add(0);
            variable_set_[5].add(6);
            variable_set_[5].add(9);

            variable_set_[6].add(5);

            variable_set_[7].add(8);
            variable_set_[7].add(0);
            variable_set_[7].add(9);

            variable_set_[8].add(0);
            variable_set_[8].add(7);

            variable_set_[9].add(7);
            variable_set_[9].add(5);
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

    m(0, 0) = 15;
    m(0, 1) = -0.5;
    m(0, 2) = -4;
    m(0, 4) = -1;
    m(0, 5) = -1.5;
    m(0, 7) = -2;
    m(0, 8) = -3;
    m(0, 9) = 0.5;

    m(1, 0) = -3;
    m(1, 1) = 15;
    m(1, 2) = 1;

    m(2, 0) = -7;
    m(2, 1) = 2;
    m(2, 2) = 15;

    m(3, 3) = 18;
    m(3, 4) = -10;

    m(4, 0) = -11;
    m(4, 3) = -7;
    m(4, 4) = 19;

    m(5, 0) = -7;
    m(5, 5) = 20;
    m(5, 6) = -5;
    m(5, 9) = 2;

    m(6, 5) = -10;
    m(6, 6) = 21;

    m(7, 0) = -11;
    m(7, 7) = 22;
    m(7, 8) = -6;
    m(7, 9) = 4;

    m(8, 0) = -11;
    m(8, 7) = 2;
    m(8, 8) = 23;

    m(9, 0) = -1;
    m(9, 5) = -13;
    m(9, 7) = 2;
    m(9, 9) = 24;

    m.finalize();

    // set up type of variables
    VariableCategorizer variable_categorizer{10};
    variable_categorizer.SetType(0, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(2, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(4, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(5, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(6, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(7, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(8, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(9, VariableCategorizer::Type::COARSE);


    StrengthPolicyMock strength_policy;
    AMGStandardInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_NEAR(0.32888259464578978, interpolation_operator(0, 0), 1E-7);
    ASSERT_NEAR(0.030291817927901688, interpolation_operator(0, 1), 1E-8);
    ASSERT_NEAR(0.030832743248042790, interpolation_operator(0, 2), 1E-10);
    ASSERT_NEAR(0.29150957252695003, interpolation_operator(0, 3), 1E-9);
    ASSERT_NEAR(-0.078600513862221860, interpolation_operator(0, 4), 1E-9);
}
