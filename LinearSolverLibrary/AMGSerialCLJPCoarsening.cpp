#include "pch.h"

#include "AMGSerialCLJPCoarsening.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


AMGSerialCLJPCoarsening::AMGSerialCLJPCoarsening(SparseMatrix2D const & m, AMGStrengthPolicyImpl const & strength_policy, IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer & categorizer)
    :
    m_{m},
    strength_graph_{strength_policy},
    variable_influence_accessor_{variable_influence_accessor},
    categorizer_{categorizer} {

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
            setFineNodes(j);
        }
    }
}

void
AMGSerialCLJPCoarsening::setFineNodes(size_type j) {
    auto neighborhood = strength_graph_.getNeighborhood(j);
    for (auto k : *neighborhood) {
        if (categorizer_.GetType(k) == VariableCategorizer::Type::UNDEFINED) {
            if (weights_[k] < 1) {
                std::cout << k << " = F" << std::endl;
                categorizer_.SetType(k, VariableCategorizer::Type::FINE);
            }
        }
    }
}

void
AMGSerialCLJPCoarsening::printWeights() const {
    int const chunk_size = 4;
    auto chunks = weights_.size() / chunk_size;
    for (size_t i = 0; i < chunks; ++i) {
        for (size_t col = 0; col < chunk_size; ++col) {
            auto index = i * chunk_size + col;
            std::cout << boost::format(" %2i: %2.2f --  ") % index % weights_.at(index);
        }
        std::cout << std::endl;
    }
    chunks = weights_.size() % chunk_size;
    auto index = weights_.size() - chunks;
    for (size_t col = 0; col < chunks; ++col)
        std::cout << boost::format(" %2i: %2.2f --  ") % index % weights_.at(index);
}

void
AMGSerialCLJPCoarsening::initialWeightInitialization() {
    std::random_device rd;
    std::mt19937 random_number_generator(rd());
    std::uniform_real_distribution<> distribution(0, 1);

    for (size_type i = 0; i < m_.cols(); ++i) {
        // Get the number of variables that variable i strongly influences.
        // The more there are, the more likely i is to become a coarse variable.
        auto strongly_influenced = strength_graph_.getStronglyInfluenced(i);









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
    auto const & influencers = strength_graph_.getStrongInfluencers(k);
    for (auto j : *influencers) {
        --weights_[j];
        strength_graph_.removeEdge(k, j);
    }

    // Heuristic 2: deals with the nodes that k strongly influences
    auto const & influenced = strength_graph_.getStronglyInfluenced2(k);
    for (auto j : *influenced) {
        strength_graph_.removeEdge(j, k);
        auto const & j_influences = strength_graph_.getStronglyInfluenced(j);
        for (auto i : *j_influences) {
//            if (strength_graph_.hasEdge(i, k)) {

                auto const & influenced2 = strength_graph_.getStronglyInfluenced2(k);
                if (influenced2->contains(i)) {


                --weights_[j];
                strength_graph_.removeEdge(i, j);
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
        if (categorizer_.GetType(i) != VariableCategorizer::Type::UNDEFINED)
            continue;
        auto weight = weights_.at(i);
        auto ncnt = 0;
        auto nremovedEdges = 0;
        auto neighborhood = strength_graph_.getNeighborhood(i);
        for (auto j : *neighborhood) {
            if (strength_graph_.hasEdge(i, j) == false) {
                if (strength_graph_.hasEdge(j, i) == false) {
                    ++nremovedEdges;
                    continue;
                }
            }
            auto other_weight = weights_.at(j);
            if (weight > other_weight)
                ++ncnt;
        }
        if (ncnt == neighborhood->size() - nremovedEdges)
            independent_set.push_back(i);
    }
    return independent_set;
}
