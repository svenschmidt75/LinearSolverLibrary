#include "pch.h"

#include "LinearSolverLibrary/AMGSerialCLJPCoarsening.h"
#include "LinearSolverLibrary/VariableCategorizer.h"
#include "..\common/Utility.hpp"
#include <LinearSolverLibrary/AMGStrengthGraph.h>


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


namespace {

    class StrengthPolicyMock : public AMGStrengthPolicyImpl {
    public:
        StrengthPolicyMock() {
            // from "Improving Coarsening and Interpolation for Algebraic Multigrid",
            // Jeffrey S. Butler, PhD Thesis, 2006
            // page 52, fig. 4.4

            auto top_inner_nodes = {1, 2, 3, 4, 5};
            auto bottom_inner_nodes = {43, 44, 45, 46, 47};
            auto left_inner_nodes = {7, 14, 21, 28, 35};
            auto right_inner_nodes = { 13, 20, 27, 34, 41 };
            auto inner_nodes = {8, 9, 10, 11, 12, 15, 16, 17, 18, 19, 22, 23, 24, 25, 26, 29, 30, 31, 32, 33, 36, 37, 38, 39, 40};

            // corner nodes
            variable_set_[0].add(1);
            variable_set_[0].add(7);
            variable_set_[0].add(8);

            variable_set_[6].add(5);
            variable_set_[6].add(12);
            variable_set_[6].add(13);

            variable_set_[42].add(35);
            variable_set_[42].add(36);
            variable_set_[42].add(43);

            variable_set_[48].add(40);
            variable_set_[48].add(41);
            variable_set_[48].add(47);

            for (auto node : top_inner_nodes) {
                variable_set_[node].add(node - 1);
                variable_set_[node].add(node + 7 - 1);
                variable_set_[node].add(node + 7);
                variable_set_[node].add(node + 7 + 1);
                variable_set_[node].add(node + 1);
            }

            for (auto node : bottom_inner_nodes) {
                variable_set_[node].add(node - 1);
                variable_set_[node].add(node - 7 - 1);
                variable_set_[node].add(node - 7);
                variable_set_[node].add(node - 7 + 1);
                variable_set_[node].add(node + 1);
            }

            for (auto node : left_inner_nodes) {
                variable_set_[node].add(node - 7);
                variable_set_[node].add(node - 7 + 1);
                variable_set_[node].add(node + 1);
                variable_set_[node].add(node + 7 + 1);
                variable_set_[node].add(node + 7);
            }

            for (auto node : right_inner_nodes) {
                variable_set_[node].add(node - 7);
                variable_set_[node].add(node - 7 - 1);
                variable_set_[node].add(node - 1);
                variable_set_[node].add(node + 7 - 1);
                variable_set_[node].add(node + 7);
            }

            for (auto node : inner_nodes) {
                variable_set_[node].add(node - 1);
                variable_set_[node].add(node - 7 - 1);
                variable_set_[node].add(node - 7);
                variable_set_[node].add(node + 7 + 1);
                variable_set_[node].add(node + 1);
                variable_set_[node].add(node + 7 + 1);
                variable_set_[node].add(node + 7);
                variable_set_[node].add(node + 7 - 1);
            }

        }

        std::unique_ptr<IVariableSet>
        getStrongInfluencers(IMatrix2D::size_type variable) const override {
            return std::make_unique<VariableSet>(variable_set_[variable]);
        }

        std::unique_ptr<IVariableSet>
        getStronglyInfluenced(IMatrix2D::size_type variable) const override {
            // find all variables that 'variable' strongly influences
            auto variables = std::make_unique<VariableSet>();
            for (auto const & pair : variable_set_) {
                if (pair.second.contains(variable))
                    variables->add(pair.first);
            }
            return common_NS::convert<IVariableSet>(variables);
        }

    private:
        mutable std::map<IMatrix2D::size_type, VariableSet> variable_set_;
    };

}

class AMGSerialCLJPCoarseningButlerDissPage56Test : public Test {
public:
    using size_type = IMatrix2D::size_type;

public:
    void SetUp() override {
        m_ = SparseMatrix2D{7 * 7};
        m_.finalize();

        variable_categorizer_.reset(new VariableCategorizer(m_.rows()));
        strength_graph_.reset(new AMGStrengthGraph(strength_policy_mock_));
        VariableInfluenceAccessor influence_accessor(*strength_graph_, *variable_categorizer_);
        coarsening_.reset(new AMGSerialCLJPCoarsening(m_, *strength_graph_, influence_accessor, *variable_categorizer_));

        coarsening_->weights_[0] = 3.7;
        coarsening_->weights_[1] = 5.3;
        coarsening_->weights_[2] = 5.0;
        coarsening_->weights_[3] = 5.9;
        coarsening_->weights_[4] = 5.4;
        coarsening_->weights_[5] = 5.3;
        coarsening_->weights_[6] = 3.4;
        coarsening_->weights_[7] = 5.2;
        coarsening_->weights_[8] = 8.0;
        coarsening_->weights_[9] = 8.5;
        coarsening_->weights_[10] = 8.2;
        coarsening_->weights_[11] = 8.6;
        coarsening_->weights_[12] = 8.9;
        coarsening_->weights_[13] = 5.1;
        coarsening_->weights_[14] = 5.9;
        coarsening_->weights_[15] = 8.1;
        coarsening_->weights_[16] = 8.8;
        coarsening_->weights_[17] = 8.9;
        coarsening_->weights_[18] = 8.4;
        coarsening_->weights_[19] = 8.2;
        coarsening_->weights_[20] = 5.9;
        coarsening_->weights_[21] = 5.7;
        coarsening_->weights_[22] = 8.6;
        coarsening_->weights_[23] = 8.3;
        coarsening_->weights_[24] = 8.8;
        coarsening_->weights_[25] = 8.3;
        coarsening_->weights_[26] = 8.1;
        coarsening_->weights_[27] = 5.0;
        coarsening_->weights_[28] = 5.3;
        coarsening_->weights_[29] = 8.7;
        coarsening_->weights_[30] = 8.3;
        coarsening_->weights_[31] = 8.4;
        coarsening_->weights_[32] = 8.3;
        coarsening_->weights_[33] = 8.8;
        coarsening_->weights_[34] = 5.9;
        coarsening_->weights_[35] = 5.0;
        coarsening_->weights_[36] = 8.8;
        coarsening_->weights_[37] = 8.5;
        coarsening_->weights_[38] = 8.6;
        coarsening_->weights_[39] = 8.7;
        coarsening_->weights_[40] = 8.9;
        coarsening_->weights_[41] = 5.3;
        coarsening_->weights_[42] = 3.2;
        coarsening_->weights_[43] = 5.6;
        coarsening_->weights_[44] = 5.8;
        coarsening_->weights_[45] = 5.6;
        coarsening_->weights_[46] = 5.9;
        coarsening_->weights_[47] = 5.9;
        coarsening_->weights_[48] = 3.0;
    }

public:
    SparseMatrix2D                           m_;
    StrengthPolicyMock                       strength_policy_mock_;
    std::unique_ptr<AMGStrengthGraph>        strength_graph_;
    std::unique_ptr<VariableCategorizer>     variable_categorizer_;
    std::unique_ptr<AMGSerialCLJPCoarsening> coarsening_;
};


TEST_F(AMGSerialCLJPCoarseningButlerDissPage56Test, TestInitialIndependentSet) {
    auto independent_set = coarsening_->selectIndependentSet();
    ASSERT_THAT(independent_set, ElementsAre(12, 17, 36, 40));
}

TEST_F(AMGSerialCLJPCoarseningButlerDissPage56Test, TestEdgesOfNode5) {
    coarsening_->categorizer_.SetType(12, VariableCategorizer::Type::COARSE);
    coarsening_->updateWeights(12);
    coarsening_->setFineNodes(12);

    // node 5 is detached from the strength graph
    ASSERT_EQ(5, coarsening_->strength_graph_.nEdgesRemoved(5));
}

TEST_F(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep1) {
    // create the state after one application of CLJP
    coarsening_->categorizer_.SetType(12, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(17, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(36, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(40, VariableCategorizer::Type::COARSE);
    coarsening_->updateWeights(12);
    coarsening_->setFineNodes(12);
    coarsening_->updateWeights(17);
    coarsening_->setFineNodes(17);
    coarsening_->updateWeights(36);
    coarsening_->setFineNodes(36);
    coarsening_->updateWeights(40);
    coarsening_->setFineNodes(40);
}

TEST_F(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep2) {
    // create the state after one application of CLJP

    // phase 1
    coarsening_->categorizer_.SetType(12, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(17, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(36, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(40, VariableCategorizer::Type::COARSE);
    coarsening_->updateWeights(12);
    coarsening_->setFineNodes(12);
    coarsening_->updateWeights(17);
    coarsening_->setFineNodes(17);
    coarsening_->updateWeights(36);
    coarsening_->setFineNodes(36);
    coarsening_->updateWeights(40);
    coarsening_->setFineNodes(40);

    coarsening_->printWeights();

    // phase 2
    auto independent_set = coarsening_->selectIndependentSet();
    ASSERT_THAT(independent_set, ElementsAre(3, 22, 26, 38));
}

TEST_F(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep3) {
    // create the state after one application of CLJP

    // phase 1
    coarsening_->categorizer_.SetType(12, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(17, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(36, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(40, VariableCategorizer::Type::COARSE);
    coarsening_->updateWeights(12);
    coarsening_->setFineNodes(12);
    coarsening_->updateWeights(17);
    coarsening_->setFineNodes(17);
    coarsening_->updateWeights(36);
    coarsening_->setFineNodes(36);
    coarsening_->updateWeights(40);
    coarsening_->setFineNodes(40);

    coarsening_->printWeights();

    // phase 2
    auto independent_set = coarsening_->selectIndependentSet();
    ASSERT_THAT(independent_set, ElementsAre(3, 22, 26, 38));
    coarsening_->categorizer_.SetType(3, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(22, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(26, VariableCategorizer::Type::COARSE);
    coarsening_->categorizer_.SetType(38, VariableCategorizer::Type::COARSE);
    coarsening_->updateWeights(3);
    coarsening_->setFineNodes(3);
    coarsening_->updateWeights(22);
    coarsening_->setFineNodes(22);
    coarsening_->updateWeights(26);
    coarsening_->setFineNodes(26);
    coarsening_->updateWeights(38);
    coarsening_->setFineNodes(38);

    coarsening_->printWeights();

    // phase 3
    independent_set = coarsening_->selectIndependentSet();
    ASSERT_THAT(independent_set, ElementsAre(8, 24));
}

TEST_F(AMGSerialCLJPCoarseningButlerDissPage56Test, TestFullCoarsening) {
    coarsening_->coarsen();

    variable_categorizer_->print();
}
