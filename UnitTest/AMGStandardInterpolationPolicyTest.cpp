#include "pch.h"

#include "LinearSolverLibrary/AMGStandardInterpolationPolicy.h"
#include "LinearSolverLibrary/VariableCategorizer.h"
#include <UnitTest/ComplexGraphTest.hpp>


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

    AMGStandardInterpolationPolicy interpolation_policy;
    ASSERT_TRUE(interpolation_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = interpolation_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(2.0 / 3.0, interpolation_operator(1, 0));
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(1, 1));
}

TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithMock) {
    // same test as above, but with mocked strength policy
    class StrengthPolicyMock : public IAMGStrengthPolicy {
    public:
        StrengthPolicyMock() {
            variable_set_[0].add(1);
            
            variable_set_[1].add(0);
            variable_set_[1].add(2);

            variable_set_[2].add(1);
            variable_set_[2].add(3);

            variable_set_[3].add(2);
        }

        std::unique_ptr<IVariableSet>
        getStrongInfluencers(IMatrix2D::size_type variable) const override {
            return std::make_unique<VariableSet>(variable_set_[variable]);
        }

        std::unique_ptr<IVariableSet>
        getStronglyInfluenced(IMatrix2D::size_type variable) const override {
            return nullptr;
        }

        void
        exportToGraphviz() const override {}

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
    AMGStandardInterpolationPolicy interpolation_policy;
    ASSERT_TRUE(interpolation_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = interpolation_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(2.0 / 3.0, interpolation_operator(1, 0));
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(1, 1));
}

TEST(AMGStandardInterpolationPolicyTest, TestComplexGraph) {
    ComplexGraphTest_NS::ComplexGraphTest<AMGStandardInterpolationPolicy> const complex_graph;
    ASSERT_NEAR(0.32888259464578978, complex_graph(0, 0), 1E-7);
    ASSERT_NEAR(0.030291817927901688, complex_graph(0, 1), 1E-8);
    ASSERT_NEAR(0.030832743248042790, complex_graph(0, 2), 1E-10);
    ASSERT_NEAR(0.29150957252695003, complex_graph(0, 3), 1E-9);
    ASSERT_NEAR(-0.078600513862221860, complex_graph(0, 4), 1E-9);
}

TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithNoCommonCNode) {
    class StrengthPolicyMock : public IAMGStrengthPolicy {
    public:
        StrengthPolicyMock() {
            variable_set_[0].add(2);

            variable_set_[1].add(2);
            variable_set_[1].add(3);

            variable_set_[2].add(0);
            variable_set_[2].add(3);

            variable_set_[3].add(2);
            variable_set_[3].add(1);
        }

        std::unique_ptr<IVariableSet>
        getStrongInfluencers(IMatrix2D::size_type variable) const override {
            return std::make_unique<VariableSet>(variable_set_[variable]);
        }

        std::unique_ptr<IVariableSet>
        getStronglyInfluenced(IMatrix2D::size_type variable) const override {
            return nullptr;
        }

        void
        exportToGraphviz() const override {}

    private:
        mutable std::map<IMatrix2D::size_type, VariableSet> variable_set_;
    };



    SparseMatrix2D m{4};

    m(0, 0) = 5;
    m(0, 2) = -2;

    m(1, 1) = 5;
    m(1, 2) = -1;
    m(1, 3) = -2;

    m(2, 0) = -2;
    m(2, 1) = -1;
    m(2, 2) = 5;
    m(2, 3) = -2;

    m(3, 1) = -2;
    m(3, 2) = -3;
    m(3, 3) = 5;

    m.finalize();

    // set up type of variables
    VariableCategorizer variable_categorizer{4};
    variable_categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(1, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(2, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::FINE);


    StrengthPolicyMock strength_policy;
    AMGStandardInterpolationPolicy interpolation_policy;
    ASSERT_TRUE(interpolation_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = interpolation_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_NEAR(10.0 / 19.0, interpolation_operator(2, 0), 1E-7);
    ASSERT_NEAR(9.0 / 19.0, interpolation_operator(2, 1), 1E-8);
}

TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnectionWithCommonCNode) {
    class StrengthPolicyMock : public IAMGStrengthPolicy {
    public:
        StrengthPolicyMock() {
            variable_set_[0].add(2);
            variable_set_[0].add(3);

            variable_set_[1].add(2);
            variable_set_[1].add(3);

            variable_set_[2].add(0);
            variable_set_[2].add(3);

            variable_set_[3].add(0);
            variable_set_[3].add(1);
            variable_set_[3].add(2);
        }

        std::unique_ptr<IVariableSet> getStrongInfluencers(IMatrix2D::size_type variable) const override {
            return std::make_unique<VariableSet>(variable_set_[variable]);
        }

        std::unique_ptr<IVariableSet> getStronglyInfluenced(IMatrix2D::size_type variable) const override {
            return nullptr;
        }

        void
        exportToGraphviz() const override {}

    private:
        mutable std::map<IMatrix2D::size_type, VariableSet> variable_set_;
    };



    SparseMatrix2D m{4};

    m(0, 0) = 7;
    m(0, 2) = -3;
    m(0, 3) = -3;

    m(1, 1) = 7;
    m(1, 2) = -1;
    m(1, 3) = -3;

    m(2, 0) = -3;
    m(2, 1) = -1;
    m(2, 2) = 7;
    m(2, 3) = -3;

    m(3, 0) = -3;
    m(3, 1) = -1;
    m(3, 2) = -3;
    m(3, 3) = 7;

    m.finalize();

    // set up type of variables
    VariableCategorizer variable_categorizer{4};
    variable_categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(1, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(2, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::FINE);


    StrengthPolicyMock strength_policy;
    AMGStandardInterpolationPolicy interpolation_policy;
    ASSERT_TRUE(interpolation_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = interpolation_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_NEAR(1.0, interpolation_operator(2, 0), 1E-7);
    ASSERT_NEAR(0.0, interpolation_operator(2, 1), 1E-7);
}
