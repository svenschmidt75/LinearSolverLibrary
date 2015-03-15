#include "pch.h"

#include "LinearSolverLibrary/AMGSerialCLJPCoarsening.h"
#include "LinearSolverLibrary/VariableCategorizer.h"
#include "..\common/Utility.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;



namespace {

    class StrengthPolicyMock : public AMGStrengthPolicyImpl {
    public:
        StrengthPolicyMock() {
            // from "Efficient Setup Algorithms for parallel Algebraic Multigrid",
            // David M. Alber, PhD Thesis, 2007
            // page 25, fig. 3.6
            variable_set_[0].add(1);
            variable_set_[0].add(6);
            variable_set_[0].add(8);

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
            variable_set_[17].add(16);
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
            variable_set_[28].add(22);
            variable_set_[28].add(29);

            variable_set_[29].add(28);
            variable_set_[29].add(22);
            variable_set_[29].add(23);
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

class AMGSerialCLJPCoarseningAlberDissPage25Test : public Test {
public:
    using size_type = IMatrix2D::size_type;

public:
    void SetUp() override {
        m_ = SparseMatrix2D{30};
        m_.finalize();

        variable_categorizer_.reset(new VariableCategorizer(m_.rows()));
        VariableInfluenceAccessor influence_accessor(strength_policy_, *variable_categorizer_);
        coarsening_.reset(new AMGSerialCLJPCoarsening(m_, strength_policy_, influence_accessor, *variable_categorizer_));

        coarsening_->weights_[0] = 3.4;
        coarsening_->weights_[1] = 4.7;
        coarsening_->weights_[2] = 4.1;
        coarsening_->weights_[3] = 3.0;
        coarsening_->weights_[4] = 5.3;
        coarsening_->weights_[5] = 2.9;
        coarsening_->weights_[6] = 5.6;
        coarsening_->weights_[7] = 6.7;
        coarsening_->weights_[8] = 4.8;
        coarsening_->weights_[9] = 7.2;
        coarsening_->weights_[10] = 6.8;
        coarsening_->weights_[11] = 4.4;
        coarsening_->weights_[12] = 6.1;
        coarsening_->weights_[13] = 6.5;
        coarsening_->weights_[14] = 4.6;
        coarsening_->weights_[15] = 6.4;
        coarsening_->weights_[16] = 4.1;
        coarsening_->weights_[17] = 6.3;
        coarsening_->weights_[18] = 5.7;
        coarsening_->weights_[19] = 3.9;
        coarsening_->weights_[20] = 8.3;
        coarsening_->weights_[21] = 6.5;
        coarsening_->weights_[22] = 7.0;
        coarsening_->weights_[23] = 4.6;
        coarsening_->weights_[24] = 3.2;
        coarsening_->weights_[25] = 3.6;
        coarsening_->weights_[26] = 4.8;
        coarsening_->weights_[27] = 4.2;
        coarsening_->weights_[28] = 3.7;
        coarsening_->weights_[29] = 3.5;
    }

public:
    SparseMatrix2D                           m_;
    StrengthPolicyMock                       strength_policy_;
    std::unique_ptr<VariableCategorizer>     variable_categorizer_;
    std::unique_ptr<AMGSerialCLJPCoarsening> coarsening_;
};


TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestInitialIndependentSet) {
    auto independent_set = coarsening_->selectIndependentSet();
    ASSERT_THAT(independent_set, Contains(9));
    ASSERT_THAT(independent_set, Contains(10));
    ASSERT_THAT(independent_set, Contains(20));
    ASSERT_THAT(independent_set, Contains(22));
    ASSERT_EQ(4, independent_set.size());
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode9) {
    // update weights of node 9
    coarsening_->updateWeights(9);

    // node 9 should have no more edges to other nodes, or node 9 should
    // be detached from the strength graph.
    // Basically, the number of edges removed == weight(9) (ignoring the random number part)
    ASSERT_EQ(7, coarsening_->strength_matrix_graph_.nEdgesRemoved(9));
    ASSERT_FALSE(coarsening_->strength_matrix_graph_.hasEdges(9));
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode10) {
    // update weights of node 10
    coarsening_->updateWeights(10);
    ASSERT_EQ(6, coarsening_->strength_matrix_graph_.nEdgesRemoved(10));
    ASSERT_FALSE(coarsening_->strength_matrix_graph_.hasEdges(10));
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode20) {
    // update weights of node 20
    coarsening_->updateWeights(20);
    ASSERT_EQ(8, coarsening_->strength_matrix_graph_.nEdgesRemoved(20));
    ASSERT_FALSE(coarsening_->strength_matrix_graph_.hasEdges(20));
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode22) {
    // update weights of node 22
    coarsening_->updateWeights(22);
    ASSERT_EQ(7, coarsening_->strength_matrix_graph_.nEdgesRemoved(22));
    ASSERT_FALSE(coarsening_->strength_matrix_graph_.hasEdges(22));
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode9) {
    coarsening_->updateWeights(9);
    ASSERT_NEAR(1.7, coarsening_->weights_[1], 1E-8);
    ASSERT_NEAR(1.1, coarsening_->weights_[2], 1E-8);
    ASSERT_NEAR(3.0, coarsening_->weights_[3], 1E-8);
    ASSERT_NEAR(3.7, coarsening_->weights_[7], 1E-8);
    ASSERT_NEAR(3.1, coarsening_->weights_[12], 1E-8);
    ASSERT_NEAR(3.4, coarsening_->weights_[15], 1E-8);
    ASSERT_NEAR(3.1, coarsening_->weights_[12], 1E-8);
    ASSERT_NEAR(2.6, coarsening_->weights_[6], 1E-8);
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode10) {
    coarsening_->updateWeights(10);
    ASSERT_NEAR(2.3, coarsening_->weights_[4], 1E-8);
    ASSERT_NEAR(3.7, coarsening_->weights_[7], 1E-8);
    ASSERT_NEAR(1.4, coarsening_->weights_[11], 1E-8);
    ASSERT_NEAR(3.5, coarsening_->weights_[13], 1E-8);
    ASSERT_NEAR(1.1, coarsening_->weights_[16], 1E-8);
    ASSERT_NEAR(3.3, coarsening_->weights_[17], 1E-8);
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode20) {
    coarsening_->updateWeights(20);
    ASSERT_NEAR(3.1, coarsening_->weights_[12], 1E-8);
    ASSERT_NEAR(1.6, coarsening_->weights_[14], 1E-8);
    ASSERT_NEAR(3.4, coarsening_->weights_[15], 1E-8);
    ASSERT_NEAR(0.9, coarsening_->weights_[19], 1E-8);
    ASSERT_NEAR(3.5, coarsening_->weights_[21], 1E-8);
    ASSERT_NEAR(0.2, coarsening_->weights_[24], 1E-8);
    ASSERT_NEAR(0.6, coarsening_->weights_[25], 1E-8);
    ASSERT_NEAR(1.8, coarsening_->weights_[26], 1E-8);
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode22) {
    coarsening_->updateWeights(22);
    ASSERT_NEAR(3.3, coarsening_->weights_[17], 1E-8);
    ASSERT_NEAR(2.7, coarsening_->weights_[18], 1E-8);
    ASSERT_NEAR(3.5, coarsening_->weights_[21], 1E-8);
    ASSERT_NEAR(1.6, coarsening_->weights_[23], 1E-8);
    ASSERT_NEAR(1.2, coarsening_->weights_[27], 1E-8);
    ASSERT_NEAR(0.7, coarsening_->weights_[28], 1E-8);
    ASSERT_NEAR(0.5, coarsening_->weights_[29], 1E-8);
}

TEST_F(AMGSerialCLJPCoarseningAlberDissPage25Test, TestEdgesOfNode18) {
    coarsening_->updateWeights(9);
    coarsening_->updateWeights(10);
    coarsening_->updateWeights(20);
    coarsening_->updateWeights(22);
//    ASSERT_NEAR(1.0, coarsening_->weights_[18], 1E-2);

    //ASSERT_EQ(7, coarsening_->strength_matrix_graph_.nEdgesRemoved(18));

    //ASSERT_GE(independent_set.size(), 3);
    //ASSERT_LE(independent_set.size(), 4);

}
