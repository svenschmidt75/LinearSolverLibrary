#include "pch.h"

#include "AMGStrengthGraph.h"


using namespace LinearSolverLibrary_NS;


AMGStrengthGraph::AMGStrengthGraph(AMGStrengthPolicyImpl const & strength_policy)
    :
    strength_policy_{ strength_policy } {}

bool
AMGStrengthGraph::hasEdge(size_type v1, size_type v2) const {
    auto const & influencers = strength_policy_.getStrongInfluencers(v1);
    if (influencers->contains(v2)) {
        // v1 strongly depends on v2, or v2 strongly influences v1
        auto const & edges_it = removed_edges_.find(v1);
        if (edges_it == std::cend(removed_edges_))
            return true;
        auto index = edges_it->second.find(v2);
        return index == std::cend(edges_it->second);
    }
    return false;
}

void
AMGStrengthGraph::removeEdge(size_type v1, size_type v2) {
    removed_edges_[v1].insert(v2);
}

AMGStrengthGraph::size_type
AMGStrengthGraph::nEdgesRemoved(size_type v) const {
    auto const & edges_it = removed_edges_.find(v);
    return edges_it == std::cend(removed_edges_) ? 0 : edges_it->second.size();
}

bool
AMGStrengthGraph::hasEdges(size_type v) const {
    auto neighborhood = strength_policy_.getNeighborhood(v);
    for (auto neighbor : *neighborhood) {
        auto const & edges_it = removed_edges_.find(neighbor);
        if (edges_it == std::cend(removed_edges_))
            return true;
    }
    return false;
}
