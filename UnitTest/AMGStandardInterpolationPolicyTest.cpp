#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGStandardInterpolationPolicy.h"
#include "LinearSolverLibrary/IVariableCategorizer.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnection) {
    // Example taken from "Distance-Two Interpolation for Parallel Algebraic Multigrid",
    // H. De Sterck, R. Falgout, J. Nolting, U.M. Yang,
    // May 10, 2007
    // page 7 and 9, eq (4.7)
    // // Matrix based on fig. 4.2, page 8
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
    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(2.0 / 3.0, interpolation_operator(1, 0));
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(1, 1));
}

TEST(AMGStandardInterpolationPolicyTest, TestStrongFFConnection_Ex) {

    class VariableCategorizerMock : public IVariableCategorizer {
    public:
        Type GetType(size_type variable) const override {
            switch (variable) {
            case 0:
                return IVariableCategorizer::Type::COARSE;

            case 1:
                return IVariableCategorizer::Type::FINE;

            case 2:
                return IVariableCategorizer::Type::FINE;

            case 3:
                return IVariableCategorizer::Type::COARSE;

            default:
                throw std::invalid_argument ("AMGStandardInterpolationPolicyTest::TestStrongFFConnection_Ex");
            }
        }

        void SetType(size_type variable, Type type) override {
        }

        size_type NumberOfVariables() const override {
            return 4;
        }

        void print() const override {}
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

    VariableCategorizerMock variable_categorizer;

    //VariableCategorizer variable_categorizer{4};
    //variable_categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    //variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    //variable_categorizer.SetType(2, VariableCategorizer::Type::FINE);
    //variable_categorizer.SetType(3, VariableCategorizer::Type::COARSE);

//    AMGStandardStrengthPolicy strength_policy{m};

    AMGStandardInterpolationPolicy splitting_policy;
//    ASSERT_TRUE(splitting_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    auto const & interpolation_operator = splitting_policy.Interpolator();
    interpolation_operator.print();
    ASSERT_DOUBLE_EQ(2.0 / 3.0, interpolation_operator(1, 0));
    ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(1, 1));
}
