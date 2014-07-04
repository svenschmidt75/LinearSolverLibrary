/*
 * Name  : MultigridTutorialExampleP144Test
 * Path  :
 * Use   : Used for interpolation operator testing.
 *         Example graph from 'Multigrid Tutorial', page 144, fig. 8.3.
 * Author: Sven Schmidt
 * Date  : 07/04/2014
 */
#pragma once
#include <LinearSolverLibrary/AMGExtendedPlusInterpolationPolicy.h>

namespace MultigridTutorialExampleP144Test_NS {

    class MultigridTutorialExampleP144Test {
    public:
        class StrengthPolicyMock : public LinearSolverLibrary_NS::IAMGStandardStrengthPolicy {
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

            std::shared_ptr<LinearSolverLibrary_NS::IVariableSet> GetInfluencedByVariables(IMatrix2D::size_type variable) const override {
                return std::make_shared<LinearSolverLibrary_NS::VariableSet>(variable_set_[variable]);
            }

            std::shared_ptr<LinearSolverLibrary_NS::IVariableSet> GetDependentOnVariables(IMatrix2D::size_type variable) const override {
                return nullptr;
            }

        private:
            mutable std::map<IMatrix2D::size_type, LinearSolverLibrary_NS::VariableSet> variable_set_;
        };

    public:
        static StrengthPolicyMock GetStrengthPolicy() {
            return StrengthPolicyMock{};
        }

        static LinearSolverLibrary_NS::VariableCategorizer GetVariableCategorizer() {
            // Example graph from 'Multigrid Tutorial', page 144, fig. 8.3.
            LinearSolverLibrary_NS::VariableCategorizer variable_categorizer{9};
            variable_categorizer.SetType(0, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(1, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);
            variable_categorizer.SetType(2, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(3, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);
            variable_categorizer.SetType(4, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(5, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);
            variable_categorizer.SetType(6, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            variable_categorizer.SetType(7, LinearSolverLibrary_NS::VariableCategorizer::Type::COARSE);
            variable_categorizer.SetType(8, LinearSolverLibrary_NS::VariableCategorizer::Type::FINE);
            return variable_categorizer;
        }
    };

} // namespace MultigridTutorialExampleP144Test_NS
