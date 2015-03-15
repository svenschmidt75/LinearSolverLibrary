#include "pch.h"

#include "AMGSerialCLJPCoarsening.h"

namespace LinearSolverLibrary_NS{
class IAMGStrengthPolicy;
}

using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


AMGSerialCLJPCoarsening::AMGSerialCLJPCoarsening(SparseMatrix2D const & m, AMGStrengthPolicyImpl const & strength_policy, IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer & categorizer)
    :
    m_{m},
    strength_policy_{strength_policy},
    variable_influence_accessor_{variable_influence_accessor},
    categorizer_{categorizer},
    strength_matrix_graph_{strength_policy} {

    initialWeightInitialization();
}

void
AMGSerialCLJPCoarsening::coarsen() {

    std::cout << std::endl;

    for (;;) {
        auto independent_set = selectIndependentSet();

        size_type n_coarse = 0;

        for (auto variable : independent_set) {
            if (categorizer_.GetType(variable) == VariableCategorizer::Type::UNDEFINED) {
                categorizer_.SetType(variable, VariableCategorizer::Type::COARSE);
                ++n_coarse;
                std::cout << variable << " = C" << std::endl;
            }
        }

        if (n_coarse == 0)
            break;

        for (auto j : independent_set) {
            updateWeights(j);

            auto neighborhood = strength_policy_.getNeighborhood(j);
            for (auto k : *neighborhood) {
                if (categorizer_.GetType(k) == VariableCategorizer::Type::UNDEFINED) {
                    if (weights_[k] < 1) {
                        std::cout << k << " = F" << std::endl;
                        categorizer_.SetType(k, VariableCategorizer::Type::FINE);
                    }
                }
            }
        }
    }
}

void
AMGSerialCLJPCoarsening::initialWeightInitialization() {
    std::random_device rd;
    std::mt19937 random_number_generator(rd());
    std::uniform_real_distribution<> distribution(0, 1);

    for (size_type i = 0; i < m_.cols(); ++i) {
        // Get the number of variables that variable i strongly influences.
        // The more there are, the more likely i is to become a coarse variable.
        auto strongly_influenced = strength_policy_.getStronglyInfluenced(i);









        // TODO SS: What about if a variable is not influenced by any variable?
        // => FINE








        // generate a random number, so no two variables with the same number of
        // other variables that they influence, end up with the same weight
        auto rnd = distribution(random_number_generator);

        weights_[i] = strongly_influenced->size() + rnd;
    }
}

void
AMGSerialCLJPCoarsening::updateWeights(size_type k) {
    common_NS::reporting::checkUppderBound(k, m_.cols() - 1);

    // iterate over non-removed edges!

    // Heuristic 1: deals with the nodes that k strongly depends upon
    auto const & influencers = strength_policy_.getStrongInfluencers(k);
    for (auto j : *influencers) {
        if (strength_matrix_graph_.hasEdge(k, j)) {
            --weights_[j];
            strength_matrix_graph_.removeEdge(k, j);
        }
    }

    // Heuristic 2: deals with the nodes that k strongly influences
    auto const & influenced = strength_policy_.getStronglyInfluenced(k);
    for (auto j : *influenced) {
        strength_matrix_graph_.removeEdge(j, k);
        auto const & j_influences = strength_policy_.getStronglyInfluenced(j);
        for (auto i : *j_influences) {
            if (influenced->contains(i)) {
                strength_matrix_graph_.removeEdge(i, j);
                --weights_[j];
            }
        }
    }
}

std::vector<AMGSerialCLJPCoarsening::size_type>
AMGSerialCLJPCoarsening::selectIndependentSet() const {
    /* If a variable has greater weight than any of its neighbors, it is
       added to the independent_set, as it is a candidate for becoming a
       coarse node as a high weight indicates it influences many other
       variables.

       Each two nodes in the independent set are separated by at least
       one node!
       Because the set contains more than one element, these nodes can
       be processed concurrently.
     */
    std::vector<size_type> independent_set;
    for (size_type i = 0; i < m_.cols(); ++i) {
        auto weight = weights_.at(i);
        auto ncnt = 0;
        auto neighborhood = strength_policy_.getNeighborhood(i);
        for (auto j : *neighborhood) {
            auto other_weight = weights_.at(j);
            if (weight > other_weight)
                ++ncnt;
        }
        if (ncnt == neighborhood->size())
            independent_set.push_back(i);
    }
    return independent_set;
}
