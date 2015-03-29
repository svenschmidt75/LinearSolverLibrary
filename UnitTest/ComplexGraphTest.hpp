/*
 * Name  : ComplexGraphTest
 * Path  :
 * Use   : Used for interpolation operator testing.
 * Author: Sven Schmidt
 * Date  : 07/04/2014
 */
#pragma once


namespace ComplexGraphTest_NS {

    template<typename INTERPOLATION_POLICY>
    class ComplexGraphTest {
    public:
        ComplexGraphTest() {
            class StrengthPolicyMock : public LinearSolverLibrary_NS::IAMGStrengthPolicy {
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

                std::unique_ptr<LinearSolverLibrary_NS::IVariableSet>
                getStrongInfluencers(IMatrix2D::size_type variable) const override {
                    return std::make_unique<LinearSolverLibrary_NS::VariableSet>(variable_set_[variable]);
                }

                std::unique_ptr<LinearSolverLibrary_NS::IVariableSet>
                getStronglyInfluenced(IMatrix2D::size_type variable) const override {
                    return nullptr;
                }

            private:
                mutable std::map<IMatrix2D::size_type, LinearSolverLibrary_NS::VariableSet> variable_set_;
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
            LinearSolverLibrary_NS::VariableCategorizer variable_categorizer{10};
            variable_categorizer.SetType(0, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(1, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(2, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);
            variable_categorizer.SetType(3, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);
            variable_categorizer.SetType(4, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(5, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(6, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);
            variable_categorizer.SetType(7, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(8, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);
            variable_categorizer.SetType(9, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);


            StrengthPolicyMock strength_policy;
            interpolation_policy_.ComputeInterpolationOperator(m, strength_policy, variable_categorizer);
        }

        double operator()(IMatrix2D::size_type row, IMatrix2D::size_type column) const {
            auto const & interpolator = interpolation_policy_.Interpolator();
            return interpolator(row, column);
        }

    private:
            INTERPOLATION_POLICY interpolation_policy_;
    };

} // namespace ComplexGraphTest_NS
