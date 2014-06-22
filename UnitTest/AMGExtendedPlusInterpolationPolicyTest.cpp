#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGExtendedPlusInterpolationPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(AMGExtendedPlusInterpolationPolicyTest, TestStrongFFConnection) {
    // Example taken from "Distance-Two Interpolation for Parallel Algebraic Multigrid",
    // H. De Sterck, R. Falgout, J. Nolting, U.M. Yang,
    // May 10, 2007
    // page 8 and 9, eq (4.10) & (4.11)
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
    VariableInfluenceAccessor influence_accessor{strength_policy, variable_categorizer};

    AMGExtendedPlusInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = splitting_policy.Interpolator();
    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(2.0 / 3.0, interpolation_operator(1, 0));
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(1, 1));
}








namespace {

    class StrengthPolicyMock : public IAMGStandardStrengthPolicy {
    public:
        StrengthPolicyMock() {
            variable_set_[0].add(4);

            variable_set_[1].add(4);

            variable_set_[2].add(4);

            variable_set_[3].add(4);

            variable_set_[4].add(0);
            variable_set_[4].add(1);
            variable_set_[4].add(2);
            variable_set_[4].add(3);
            variable_set_[4].add(5);
            variable_set_[4].add(7);

            variable_set_[5].add(4);

            variable_set_[6].add(4);

            variable_set_[7].add(4);

            variable_set_[8].add(4);
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

    StrengthPolicyMock GetStrengthPolicy() {
        return StrengthPolicyMock{};
    }

    VariableCategorizer GetVariableCategorizer() {
        // Example graph from 'Multigrid Tutorial', page 144, fig. 8.3.
        VariableCategorizer variable_categorizer{9};
        variable_categorizer.SetType(0, VariableCategorizer::Type::FINE);
        variable_categorizer.SetType(1, VariableCategorizer::Type::COARSE);
        variable_categorizer.SetType(2, VariableCategorizer::Type::FINE);
        variable_categorizer.SetType(3, VariableCategorizer::Type::COARSE);
        variable_categorizer.SetType(4, VariableCategorizer::Type::FINE);
        variable_categorizer.SetType(5, VariableCategorizer::Type::COARSE);
        variable_categorizer.SetType(6, VariableCategorizer::Type::FINE);
        variable_categorizer.SetType(7, VariableCategorizer::Type::COARSE);
        variable_categorizer.SetType(8, VariableCategorizer::Type::FINE);
        return variable_categorizer;
    }
}


TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithWeakConnections) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1.0 / 2.0,         -2, -1.0 / 2.0,
                       -1, 29.0 / 4.0, -1,
               -1.0 / 8.0,         -2, -1.0 / 8.0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();


    AMGExtendedPlusInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, GetStrengthPolicy(), GetVariableCategorizer()));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(4, 0));
    ASSERT_DOUBLE_EQ(33.0 / 174.0, interpolation_operator(4, 1));
    ASSERT_DOUBLE_EQ(33.0 / 174.0, interpolation_operator(4, 2));
    ASSERT_DOUBLE_EQ(25.0 / 87.0, interpolation_operator(4, 3));
}

TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithPositiveOffDiagonalEntries) {
 
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1.0 / 2.0,         -2, -1.0 / 2.0,
                       -1, 29.0 / 4.0, -1,
                1.0 / 8.0,         -2,  1.0 / 8.0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();


    AMGExtendedPlusInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, GetStrengthPolicy(), GetVariableCategorizer()));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(14.0 / 45.0, interpolation_operator(4, 0));
    ASSERT_DOUBLE_EQ(8.0 / 45.0, interpolation_operator(4, 1));
    ASSERT_DOUBLE_EQ(8.0 / 45.0, interpolation_operator(4, 2));
    ASSERT_DOUBLE_EQ(4.0 / 15.0, interpolation_operator(4, 3));
}

TEST(AMGExtendedPlusInterpolationPolicyTestFromMGTutorial, MadeUpExampleWithStrongPositiveOffDiagonalEntries) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1.0 / 2.0,          2, -1.0 / 2.0,
                       -1, 29.0 / 4.0, -1,
                1.0 / 8.0,          2,  1.0 / 8.0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();


    AMGExtendedPlusInterpolationPolicy splitting_policy;
    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, GetStrengthPolicy(), GetVariableCategorizer()));

    auto const & interpolation_operator = splitting_policy.Interpolator();
//    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(- 17.0 / 58.0, interpolation_operator(4, 0));
    ASSERT_DOUBLE_EQ(6.0 / 29.0, interpolation_operator(4, 1));
    ASSERT_DOUBLE_EQ(6.0 / 29.0, interpolation_operator(4, 2));
    ASSERT_DOUBLE_EQ(- 17.0 / 58, interpolation_operator(4, 3));
}
