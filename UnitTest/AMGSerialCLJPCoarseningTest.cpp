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

class AMGSerialCLJPCoarseningTest : public Test {
public:
    using size_type = IMatrix2D::size_type;

public:
    void SetUp() override {
        m_ = SparseMatrix2D{30};
        m_.finalize();

        variable_categorizer_.reset(new VariableCategorizer(m_.rows()));
        VariableInfluenceAccessor influence_accessor(strength_policy_, *variable_categorizer_);
        coarsening_.reset(new AMGSerialCLJPCoarsening(m_, strength_policy_, influence_accessor, *variable_categorizer_));
        //coarsening_->coarsen();
    }

public:
    SparseMatrix2D                           m_;
    StrengthPolicyMock                       strength_policy_;
    std::unique_ptr<VariableCategorizer>     variable_categorizer_;
    std::unique_ptr<AMGSerialCLJPCoarsening> coarsening_;
};



TEST_F(AMGSerialCLJPCoarseningTest, TestInitialWeights) {
    auto pred = [](auto expected, auto weight) {
        return weight >= expected && weight < expected + 1;
    };

    ASSERT_TRUE(pred(3, coarsening_->weights_[0]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[1]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[2]));
    ASSERT_TRUE(pred(3, coarsening_->weights_[3]));
    ASSERT_TRUE(pred(5, coarsening_->weights_[4]));
    ASSERT_TRUE(pred(2, coarsening_->weights_[5]));
    ASSERT_TRUE(pred(5, coarsening_->weights_[6]));
    ASSERT_TRUE(pred(6, coarsening_->weights_[7]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[8]));
    ASSERT_TRUE(pred(7, coarsening_->weights_[9]));
    ASSERT_TRUE(pred(6, coarsening_->weights_[10]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[11]));
    ASSERT_TRUE(pred(6, coarsening_->weights_[12]));
    ASSERT_TRUE(pred(6, coarsening_->weights_[13]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[14]));
    ASSERT_TRUE(pred(6, coarsening_->weights_[15]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[16]));
    ASSERT_TRUE(pred(6, coarsening_->weights_[17]));
    ASSERT_TRUE(pred(5, coarsening_->weights_[18]));
    ASSERT_TRUE(pred(3, coarsening_->weights_[19]));
    ASSERT_TRUE(pred(8, coarsening_->weights_[20]));
    ASSERT_TRUE(pred(6, coarsening_->weights_[21]));
    ASSERT_TRUE(pred(7, coarsening_->weights_[22]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[23]));
    ASSERT_TRUE(pred(3, coarsening_->weights_[24]));
    ASSERT_TRUE(pred(3, coarsening_->weights_[25]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[26]));
    ASSERT_TRUE(pred(4, coarsening_->weights_[27]));
    ASSERT_TRUE(pred(3, coarsening_->weights_[28]));
    ASSERT_TRUE(pred(3, coarsening_->weights_[29]));
}

TEST_F(AMGSerialCLJPCoarseningTest, TestIndependentSet) {
    auto independent_set = coarsening_->selectIndependentSet();
    EXPECT_THAT(independent_set, Contains(9));
    EXPECT_THAT(independent_set, Contains(20));
    EXPECT_THAT(independent_set, Contains(22));

    // depending on the random numbers, element 10 might also be in here
    ASSERT_GE(independent_set.size(), 3);
    ASSERT_LE(independent_set.size(), 4);

    if (independent_set.size() == 4)
        EXPECT_THAT(independent_set, Contains(10));
}

TEST_F(AMGSerialCLJPCoarseningTest, TestWeightUpdate) {
    ASSERT_TRUE(coarsening_->strength_graph_.hasEdge(20, 12));
    ASSERT_TRUE(coarsening_->strength_graph_.hasEdge(20, 15));
    ASSERT_TRUE(coarsening_->strength_graph_.hasEdge(20, 21));
    ASSERT_TRUE(coarsening_->strength_graph_.hasEdge(20, 26));
    ASSERT_TRUE(coarsening_->strength_graph_.hasEdge(20, 25));
    ASSERT_TRUE(coarsening_->strength_graph_.hasEdge(20, 24));
    ASSERT_TRUE(coarsening_->strength_graph_.hasEdge(20, 19));
    ASSERT_TRUE(coarsening_->strength_graph_.hasEdge(20, 14));

    variable_categorizer_->SetType(20, VariableCategorizer::Type::COARSE);
    coarsening_->updateWeights(20);

    ASSERT_FALSE(coarsening_->strength_graph_.hasEdge(20, 12));
    ASSERT_FALSE(coarsening_->strength_graph_.hasEdge(20, 15));
    ASSERT_FALSE(coarsening_->strength_graph_.hasEdge(20, 21));
    ASSERT_FALSE(coarsening_->strength_graph_.hasEdge(20, 26));
    ASSERT_FALSE(coarsening_->strength_graph_.hasEdge(20, 25));
    ASSERT_FALSE(coarsening_->strength_graph_.hasEdge(20, 24));
    ASSERT_FALSE(coarsening_->strength_graph_.hasEdge(20, 19));
    ASSERT_FALSE(coarsening_->strength_graph_.hasEdge(20, 14));
}

TEST_F(AMGSerialCLJPCoarseningTest, TestWeightUpdateHeuristic1) {
    class StrengthPolicyMock : public AMGStrengthPolicyImpl {
    public:
        StrengthPolicyMock() {
            // from "Efficient Setup Algorithms for parallel Algebraic Multigrid",
            // David M. Alber, PhD Thesis, 2007
            // page 24, fig. 3.5 (a)

            // node k
            variable_set_[0].add(1);

            // node i does not depend on k
            variable_set_[1].add(2);
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


    SparseMatrix2D m{2};
    m.finalize();

    StrengthPolicyMock strength_policy;

    VariableCategorizer variable_categorizer{m.rows()};
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    AMGSerialCLJPCoarsening coarsening{m, strength_policy, influence_accessor, variable_categorizer};

    ASSERT_TRUE(coarsening.strength_graph_.hasEdge(0, 1));

    // weight of undefined node i
    auto weight = coarsening.weights_[1];

    coarsening.updateWeights(0);

    ASSERT_FALSE(coarsening.strength_graph_.hasEdge(0, 1));

    // assert weight of undefined node i is reduced by 1
    ASSERT_NEAR(weight - 1.0, coarsening.weights_[1], 1E-6);
}

TEST_F(AMGSerialCLJPCoarseningTest, TestWeightUpdateHeuristic2) {
    class StrengthPolicyMock : public AMGStrengthPolicyImpl {
    public:
        StrengthPolicyMock() {
            // from "Efficient Setup Algorithms for parallel Algebraic Multigrid",
            // David M. Alber, PhD Thesis, 2007
            // page 24, fig. 3.5 (b)

            // node k
            variable_set_[0].add(3);

            // node i depends on k and j
            variable_set_[1].add(0);
            variable_set_[1].add(2);

            // node j depends on k
            variable_set_[2].add(0);
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


    enum Node_t : size_type { k = 0, i, j };


    SparseMatrix2D m{4};
    m.finalize();

    StrengthPolicyMock strength_policy;

    VariableCategorizer variable_categorizer{m.rows()};
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    AMGSerialCLJPCoarsening coarsening{m, strength_policy, influence_accessor, variable_categorizer};

    ASSERT_TRUE(coarsening.strength_graph_.hasEdge(Node_t::i, Node_t::k));
    ASSERT_TRUE(coarsening.strength_graph_.hasEdge(Node_t::j, Node_t::k));
    ASSERT_TRUE(coarsening.strength_graph_.hasEdge(Node_t::i, Node_t::j));

    // weight of undefined node i
    auto weight_i = coarsening.weights_[Node_t::i];

    // weight of undefined node 3
    auto weight_3 = coarsening.weights_[3];

    // update weights for node k
    coarsening.updateWeights(Node_t::k);

    // only edges to and from node k are removed
    ASSERT_FALSE(coarsening.strength_graph_.hasEdge(Node_t::k, Node_t::i));
    ASSERT_FALSE(coarsening.strength_graph_.hasEdge(Node_t::k, Node_t::j));
    ASSERT_FALSE(coarsening.strength_graph_.hasEdge(Node_t::k, 3));

    // edge between i and j should be removed
    ASSERT_FALSE(coarsening.strength_graph_.hasEdge(Node_t::i, Node_t::j));

    // assert weight of undefined node 3 is reduced by 1
    ASSERT_NEAR(weight_3 - 1.0, coarsening.weights_[3], 1E-6);

    // assert weight of undefined node i is unchanged
    ASSERT_NEAR(weight_i, coarsening.weights_[1], 1E-6);
}
