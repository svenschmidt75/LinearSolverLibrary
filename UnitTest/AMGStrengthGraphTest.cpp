#include "pch.h"

#include <common/Utility.hpp>
#include <LinearSolverLibrary/AMGStrengthGraph.h>


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

        void
        exportToGraphviz() const override {
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "digraph G {" << std::endl;
            for (IMatrix2D::size_type i = 0; i < variable_set_.size(); ++i) {
                auto vertices = getStrongInfluencers(i);
                for (auto target_index : *vertices)
                    std::cout << "  " << i << " -> " << target_index << std::endl;
            }
            std::cout << "}" << std::endl;
            std::cout << std::endl;
        }

    private:
        mutable std::map<IMatrix2D::size_type, VariableSet> variable_set_;
    };

}

class AMGStrengthGraphTest : public Test {
public:
    using size_type = IMatrix2D::size_type;

public:
    void SetUp() override {
        strength_graph_.reset(new AMGStrengthGraph(strength_policy_mock_));
    }

public:
    StrengthPolicyMock                strength_policy_mock_;
    std::unique_ptr<AMGStrengthGraph> strength_graph_;
};


TEST_F(AMGStrengthGraphTest, TestHasEdges) {
    ASSERT_TRUE(strength_graph_->hasEdges(20));
}

TEST_F(AMGStrengthGraphTest, TestRemoveEdgeThatDoesNotExist) {
    ASSERT_THROW(strength_graph_->removeEdge(20, 1), std::runtime_error);
}

TEST_F(AMGStrengthGraphTest, TestRemoveEdgeTwice) {
    strength_graph_->removeEdge(20, 12);
    ASSERT_THROW(strength_graph_->removeEdge(20, 12), std::runtime_error);
}

TEST_F(AMGStrengthGraphTest, TestRemoveEdge) {
    strength_graph_->removeEdge(20, 12);
    ASSERT_FALSE(strength_graph_->hasEdge(20, 12));
}

TEST_F(AMGStrengthGraphTest, TestHasEdgesWithAllEdgesRemoved) {
    strength_graph_->removeEdge(20, 12);
    strength_graph_->removeEdge(20, 14);
    strength_graph_->removeEdge(20, 15);
    strength_graph_->removeEdge(20, 19);
    strength_graph_->removeEdge(20, 21);
    strength_graph_->removeEdge(20, 24);
    strength_graph_->removeEdge(20, 25);
    strength_graph_->removeEdge(20, 26);
    ASSERT_FALSE(strength_graph_->hasEdges(20));
}

TEST_F(AMGStrengthGraphTest, TestGetStrongInfluencers) {
    strength_graph_->removeEdge(20, 12);
    auto influencers = strength_graph_->getStrongInfluencers(20);
    ASSERT_EQ(7, influencers->size());
    ASSERT_FALSE(influencers->contains(12));
    ASSERT_TRUE(influencers->contains(14));
    ASSERT_TRUE(influencers->contains(15));
    ASSERT_TRUE(influencers->contains(19));
    ASSERT_TRUE(influencers->contains(21));
    ASSERT_TRUE(influencers->contains(24));
    ASSERT_TRUE(influencers->contains(25));
    ASSERT_TRUE(influencers->contains(26));
}

TEST_F(AMGStrengthGraphTest, TestGetStrongInfluencersWithNoEdgesRemoved) {
    auto influencers = strength_graph_->getStrongInfluencers(20);
    ASSERT_EQ(8, influencers->size());
    ASSERT_TRUE(influencers->contains(12));
    ASSERT_TRUE(influencers->contains(14));
    ASSERT_TRUE(influencers->contains(15));
    ASSERT_TRUE(influencers->contains(19));
    ASSERT_TRUE(influencers->contains(21));
    ASSERT_TRUE(influencers->contains(24));
    ASSERT_TRUE(influencers->contains(25));
    ASSERT_TRUE(influencers->contains(26));
}

TEST_F(AMGStrengthGraphTest, TestGetStronglyInfluenced) {
    strength_graph_->removeEdge(12, 20);
    auto influenced = strength_graph_->getStronglyInfluenced(20);
    ASSERT_EQ(7, influenced->size());
    ASSERT_FALSE(influenced->contains(12));
    ASSERT_TRUE(influenced->contains(14));
    ASSERT_TRUE(influenced->contains(15));
    ASSERT_TRUE(influenced->contains(19));
    ASSERT_TRUE(influenced->contains(21));
    ASSERT_TRUE(influenced->contains(24));
    ASSERT_TRUE(influenced->contains(25));
    ASSERT_TRUE(influenced->contains(26));
}

TEST_F(AMGStrengthGraphTest, TestGetStronglyInfluencedWithNoEdgesRemoved) {
    auto influenced = strength_graph_->getStronglyInfluenced(20);
    ASSERT_EQ(8, influenced->size());
    ASSERT_TRUE(influenced->contains(12));
    ASSERT_TRUE(influenced->contains(14));
    ASSERT_TRUE(influenced->contains(15));
    ASSERT_TRUE(influenced->contains(19));
    ASSERT_TRUE(influenced->contains(21));
    ASSERT_TRUE(influenced->contains(24));
    ASSERT_TRUE(influenced->contains(25));
    ASSERT_TRUE(influenced->contains(26));
}
