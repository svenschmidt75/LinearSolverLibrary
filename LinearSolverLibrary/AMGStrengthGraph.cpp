#include "pch.h"

#include "AMGStrengthGraph.h"
#include "VariableSet.h"
#include <common/Utility.hpp>


using namespace LinearSolverLibrary_NS;


AMGStrengthGraph::AMGStrengthGraph(AMGStrengthPolicyImpl const & strength_policy)
    :
    strength_policy_{strength_policy} {}

std::unique_ptr<IVariableSet>
AMGStrengthGraph::getStrongInfluencers(LinAlg_NS::IMatrix2D::size_type variable) const {
    auto influencers = strength_policy_.getStrongInfluencers(variable);
    std::set<size_type> deleted_nodes;
	boost::graph_traits<Graph_t>::out_edge_iterator ei, ei_end;
    boost::tie(ei, ei_end) = boost::out_edges(variable, g_);
    for (; ei != ei_end; ++ei) {
        auto source_node = boost::target(*ei, g_);
        deleted_nodes.insert(source_node);
    }
    auto variable_set = std::make_unique<VariableSet>();
    for (auto influencer : *influencers) {
        if (deleted_nodes.find(influencer) == std::end(deleted_nodes))
            variable_set->add(influencer);
    }
    return common_NS::convert<IVariableSet>(variable_set);
}

std::unique_ptr<IVariableSet>
AMGStrengthGraph::getStronglyInfluenced(LinAlg_NS::IMatrix2D::size_type variable) const {
    auto influenced = strength_policy_.getStronglyInfluenced(variable);
    std::set<size_type> deleted_nodes;
	boost::graph_traits<Graph_t>::in_edge_iterator ei, ei_end;
    boost::tie(ei, ei_end) = boost::in_edges(variable, g_);
    for (; ei != ei_end; ++ei) {
        auto source_node = boost::source(*ei, g_);
        deleted_nodes.insert(source_node);
    }
    auto variable_set = std::make_unique<VariableSet>();
    for (auto node : *influenced) {
        if (deleted_nodes.find(node) == std::end(deleted_nodes))
            variable_set->add(node);
    }
    return common_NS::convert<IVariableSet>(variable_set);
}

bool
AMGStrengthGraph::hasEdge(size_type v1, size_type v2) const {
    auto const & influencers = strength_policy_.getStrongInfluencers(v1);
    if (influencers->contains(v2) == false)
        return false;
    if (boost::num_vertices(g_) < std::max(v1, v2))
        return true;
    auto ei = boost::edge(v1, v2, g_);
    return ei.second == false;
}

void
AMGStrengthGraph::removeEdge(size_type v1, size_type v2) {
    // Remove 'node v1 strongly depends on node v2'
    if (hasEdge(v1, v2) == false) {
        std::ostringstream stream;
        stream << boost::format("AMGStrengthGraph::removeEdge: Node %s does not have a strong dependency on node %s") % v1 % v2;
        throw std::runtime_error(stream.str());
    }
    boost::add_edge(v1, v2, g_);
}

AMGStrengthGraph::size_type
AMGStrengthGraph::nEdgesRemoved(size_type v) const {
    boost::graph_traits<Graph_t>::out_edge_iterator ei, ei_end;
    boost::tie(ei, ei_end) = boost::out_edges(v, g_);
    return std::distance(ei, ei_end);
}

bool
AMGStrengthGraph::hasEdges(size_type v) const {
	// if the number of out and in edges == number of nodes in
	// node's v neighborhood, than it has no more connections, i.e.
	// it is completely detached from the graph.
    auto neighborhood = strength_policy_.getNeighborhood(v);
    auto neighborhood_size = neighborhood->size();
    if (boost::num_vertices(g_) < v)
        return neighborhood_size > 0;
    std::set<size_type> nodes;
    boost::graph_traits<Graph_t>::out_edge_iterator ei_out, ei_out_end;
    boost::tie(ei_out, ei_out_end) = boost::out_edges(v, g_);
    std::for_each(ei_out, ei_out_end, [&nodes, this](auto edge) {
        auto target_node = boost::target(edge, g_);
        nodes.insert(target_node);
    });
    size_type nout_edges = std::distance(ei_out, ei_out_end);
    boost::graph_traits<Graph_t>::in_edge_iterator ei_in, ei_in_end;
    boost::tie(ei_in, ei_in_end) = boost::in_edges(v, g_);
    std::for_each(ei_in, ei_in_end, [&nodes, this](auto edge) {
        auto target_node = boost::source(edge, g_);
        nodes.insert(target_node);
    });
    return nodes.size() < neighborhood_size;
}
