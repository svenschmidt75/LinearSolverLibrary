#include "pch.h"

#include "GraphHelper.h"


using namespace LinearSolverLibrary_NS;


GraphHelper::GraphHelper(AMGStrengthPolicyImpl const & strength_policy)
    :
    strength_policy_{strength_policy} {}

bool
GraphHelper::hasEdge(size_type v1, size_type v2) const {
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
GraphHelper::removeEdge(size_type v1, size_type v2) {
    // the graph is undirected, so remove "both" edges
    removed_edges_[v1].insert(v2);
    removed_edges_[v2].insert(v1);
}

GraphHelper::size_type
GraphHelper::nEdgesRemoved(size_type v) const {
    auto const & edges_it = removed_edges_.find(v);
    return edges_it == std::cend(removed_edges_) ? 0 : edges_it->second.size();
}

bool
GraphHelper::hasEdges(size_type v) const {
    auto influencers = strength_policy_.getStrongInfluencers(v);
    auto influenced = strength_policy_.getStronglyInfluenced(v);

    // SS: Use set here because we do not want duplicate nodes!
    std::set<size_type> neighborhood;
    neighborhood.insert(std::cbegin(*influencers), std::cend(*influencers));
    neighborhood.insert(std::cbegin(*influenced), std::cend(*influenced));

    return false;


    //auto const & edges_it = removed_edges_.find(v);
    //return edges_it == std::cend(removed_edges_) ? 0 : edges_it->second.size();
}
