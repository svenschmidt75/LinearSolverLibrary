#include "pch.h"

#include "LinearSolverLibrary/AMGSerialCLJPCoarsening.h"
#include "LinearSolverLibrary/VariableCategorizer.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(AMGSerialCLJPCoarseningTest, Test) {
    class StrengthPolicyMock : public IAMGStandardStrengthPolicy {
    public:
        StrengthPolicyMock() {
            // from "Efficient Setup Algorithms for parallel Algebraic Multigrid",
            // David M. Alber, PhD Thesis, 2007
            // page 25, fig. 3.6
            variable_set_[0].add(1);
            variable_set_[0].add(7);
            variable_set_[0].add(9);

            variable_set_[1].add(0);
            variable_set_[1].add(2);
            variable_set_[1].add(6);
            variable_set_[1].add(9);

            variable_set_[2].add(1);
            variable_set_[2].add(3);
            variable_set_[2].add(7);
            variable_set_[2].add(9);

            variable_set_[3].add(2);
            variable_set_[3].add(4);
            variable_set_[3].add(7);

            variable_set_[4].add(3);
            variable_set_[4].add(5);
            variable_set_[4].add(7);
            variable_set_[4].add(10);
            variable_set_[4].add(11);

            variable_set_[5].add(4);
            variable_set_[5].add(11);

            variable_set_[6].add(0);
            variable_set_[6].add(1);
            variable_set_[6].add(8);
            variable_set_[6].add(9);
            variable_set_[6].add(12);

            variable_set_[7].add(2);
            variable_set_[7].add(3);
            variable_set_[7].add(4);
            variable_set_[7].add(9);
            variable_set_[7].add(13);
            variable_set_[7].add(10);

            variable_set_[8].add(0);
            variable_set_[8].add(6);
            variable_set_[8].add(12);
            variable_set_[8].add(14);

            variable_set_[9].add(1);
            variable_set_[9].add(2);
            variable_set_[9].add(6);
            variable_set_[9].add(7);
            variable_set_[9].add(12);
            variable_set_[9].add(15);
            variable_set_[9].add(13);

            variable_set_[10].add(4);
            variable_set_[10].add(7);
            variable_set_[10].add(11);
            variable_set_[10].add(13);
            variable_set_[10].add(17);
            variable_set_[10].add(16);

            variable_set_[11].add(4);
            variable_set_[11].add(5);
            variable_set_[11].add(10);
            variable_set_[11].add(16);

            variable_set_[12].add(8);
            variable_set_[12].add(6);
            variable_set_[12].add(9);
            variable_set_[12].add(14);
            variable_set_[12].add(20);
            variable_set_[12].add(15);

            variable_set_[13].add(9);
            variable_set_[13].add(7);
            variable_set_[13].add(10);
            variable_set_[13].add(15);
            variable_set_[13].add(18);
            variable_set_[13].add(17);

            variable_set_[14].add(8);
            variable_set_[14].add(12);
            variable_set_[14].add(19);
            variable_set_[14].add(20);

            variable_set_[15].add(12);
            variable_set_[15].add(9);
            variable_set_[15].add(13);
            variable_set_[15].add(20);
            variable_set_[15].add(21);
            variable_set_[15].add(18);

            variable_set_[16].add(11);
            variable_set_[16].add(10);
            variable_set_[16].add(17);
            variable_set_[16].add(23);

            variable_set_[17].add(13);
            variable_set_[17].add(10);
            variable_set_[17].add(11);
            variable_set_[17].add(18);
            variable_set_[17].add(22);
            variable_set_[17].add(23);

            variable_set_[18].add(15);
            variable_set_[18].add(13);
            variable_set_[18].add(17);
            variable_set_[18].add(21);
            variable_set_[18].add(22);

            variable_set_[19].add(14);
            variable_set_[19].add(20);
            variable_set_[19].add(24);

            variable_set_[20].add(14);
            variable_set_[20].add(12);
            variable_set_[20].add(15);
            variable_set_[20].add(19);
            variable_set_[20].add(21);
            variable_set_[20].add(24);
            variable_set_[20].add(25);
            variable_set_[20].add(26);

            variable_set_[21].add(20);
            variable_set_[21].add(15);
            variable_set_[21].add(18);
            variable_set_[21].add(22);
            variable_set_[21].add(26);
            variable_set_[21].add(27);

            variable_set_[22].add(21);
            variable_set_[22].add(18);
            variable_set_[22].add(17);
            variable_set_[22].add(23);
            variable_set_[22].add(27);
            variable_set_[22].add(28);
            variable_set_[22].add(29);

            variable_set_[23].add(16);
            variable_set_[23].add(17);
            variable_set_[23].add(22);
            variable_set_[23].add(29);

            variable_set_[24].add(19);
            variable_set_[24].add(20);
            variable_set_[24].add(25);

            variable_set_[25].add(24);
            variable_set_[25].add(20);
            variable_set_[25].add(26);

            variable_set_[26].add(25);
            variable_set_[26].add(20);
            variable_set_[26].add(21);
            variable_set_[26].add(27);

            variable_set_[27].add(26);
            variable_set_[27].add(21);
            variable_set_[27].add(22);
            variable_set_[27].add(28);

            variable_set_[28].add(27);
            variable_set_[28].add(23);
            variable_set_[28].add(29);

            variable_set_[29].add(28);
            variable_set_[29].add(22);
            variable_set_[29].add(23);
        }

        std::unique_ptr<IVariableSet> GetInfluencedByVariables(IMatrix2D::size_type variable) const override {
            return std::make_unique<VariableSet>(variable_set_[variable]);
        }

        std::unique_ptr<IVariableSet> GetDependentOnVariables(IMatrix2D::size_type variable) const override {
            // find all variables that 'variable' strongly influences
            VariableSet variables;
            for (auto const & pair : variable_set_) {
                if (pair.second.contains(variable))
                    variables.add(pair.first);
            }
            return std::make_unique<IVariableSet>(variables);
        }

    private:
        mutable std::map<IMatrix2D::size_type, VariableSet> variable_set_;
    };



    SparseMatrix2D m{30};

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

    StrengthPolicyMock strength_policy;
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    AMGSerialCLJPCoarsening coarsening(m, strength_policy, influence_accessor, variable_categorizer);
    coarsening.coarsen();

    variable_categorizer.print();

    /*
    C F C
    F C F
    C F C
    */

    //for (auto variable : { 0, 2, 4, 6, 8 }) {
    //    auto variable_type = static_cast<char>(variable_categorizer.GetType(variable));
    //    auto expected = static_cast<char>(VariableCategorizer::Type::COARSE);
    //    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong variable type", expected, variable_type);
    //}

    //for (auto variable : { 1, 3, 5, 7 }) {
    //    auto variable_type = static_cast<char>(variable_categorizer.GetType(variable));
    //    auto expected = static_cast<char>(VariableCategorizer::Type::FINE);
    //    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong variable type", expected, variable_type);
    //}




    //// set up type of variables
    //VariableCategorizer variable_categorizer{m.cols()};
    //variable_categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    //variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    //variable_categorizer.SetType(2, VariableCategorizer::Type::FINE);
    //variable_categorizer.SetType(3, VariableCategorizer::Type::COARSE);


    //StrengthPolicyMock strength_policy;
    //AMGStandardInterpolationPolicy interpolation_policy;
    //ASSERT_TRUE(interpolation_policy.ComputeInterpolationOperator(m, strength_policy, variable_categorizer));

    //auto const & interpolation_operator = interpolation_policy.Interpolator();
    ////    interpolation_operator.print();
    //ASSERT_DOUBLE_EQ(2.0 / 3.0, interpolation_operator(1, 0));
    //ASSERT_DOUBLE_EQ(1.0 / 3.0, interpolation_operator(1, 1));
}
