#include "pch.h"

#include "AMGStandardStrengthPolicy.h"
#include "VariableSet.h"
#include "..\common/Utility.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGStandardStrengthPolicy::AMGStandardStrengthPolicy(SparseMatrix2D const & m)
    :
    eps_(0.25) {

    computeConnections(m);
}

namespace {

    double computeMaxElementForRow(SparseMatrix2D const & m, IMatrix2D::size_type row) {
        ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m, row);
        BOOST_ASSERT_MSG(row_it.row() == row, "Index range error");
        auto column_it = *row_it;
        double max_value = 0;
        for (; column_it.isValid(); ++column_it) {
            if (column_it.column() == row)
                // skip diagonal element
                continue;
            double a_ij = std::abs(*column_it);
            max_value = std::max(max_value, a_ij);
        }
        return max_value;
    }

}

void
AMGStandardStrengthPolicy::computeConnectionsForVariable(SparseMatrix2D const & m, IMatrix2D::size_type i, double max_element) {
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m, i);
    BOOST_ASSERT_MSG(row_it.row() == i, "Index range error");
    auto column_it = *row_it;
    for (; column_it.isValid(); ++column_it) {
        auto j = column_it.column();
        if (j == i)
            // skip diagonal element
            continue;
        double a_ij = - *column_it;
        if (a_ij >= max_element)
            boost::add_edge(i, j, g_);
    }
}

void
AMGStandardStrengthPolicy::computeConnections(SparseMatrix2D const & m) {
    // TODO SS: This can be done in parallel for each fine variable i, as
    // only S(i, j) is modified with i = const per loop iteration.
    // Use thread private memory and at the end, assembly Si and Sit.
    for (IMatrix2D::size_type i = 0; i < m.rows(); ++i) {
        boost::add_vertex(g_);
        auto max_element = computeMaxElementForRow(m, i);
        if (max_element == 0)
            // Variable i has no strong connections to other variables,
            // hence it will become an F variable.
            continue;
        max_element *= eps_;
        computeConnectionsForVariable(m, i, max_element);
    }
}

std::unique_ptr<IVariableSet>
AMGStandardStrengthPolicy::getStrongInfluencers(IMatrix2D::size_type variable) const {
    common_NS::reporting::checkConditional(variable < boost::num_vertices(g_));
    // return the variables that strongly influence variable 'variable'
    auto variable_set = std::make_unique<VariableSet>();
    boost::graph_traits<Graph_t>::out_edge_iterator ei, ei_end;
    boost::tie(ei, ei_end) = boost::out_edges(variable, g_);
    for (; ei != ei_end; ++ei) {
        auto source_node = boost::target(*ei, g_);
        variable_set->add(source_node);
    }
    return common_NS::convert<IVariableSet>(variable_set);
}

std::unique_ptr<IVariableSet>
AMGStandardStrengthPolicy::getStronglyInfluenced(IMatrix2D::size_type variable) const {
    common_NS::reporting::checkConditional(variable < boost::num_vertices(g_));
    // return the variables that strongly influence variable 'variable'
    auto variable_set = std::make_unique<VariableSet>();
    boost::graph_traits<Graph_t>::in_edge_iterator ei, ei_end;
    boost::tie(ei, ei_end) = boost::in_edges(variable, g_);
    for (; ei != ei_end; ++ei) {
        auto source_node = boost::source(*ei, g_);
        variable_set->add(source_node);
    }
    return common_NS::convert<IVariableSet>(variable_set);
}

void
AMGStandardStrengthPolicy::exportToGraphviz() const {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "digraph G {" << std::endl;
    auto edges = boost::edges(g_);
    auto ei = edges.first;
    auto eend = edges.second;
    for (; ei != eend; ++ei)
        std::cout << "  " << boost::source(*ei, g_) << " -> " << boost::target(*ei, g_) << std::endl;
    std::cout << "}" << std::endl;
    std::cout << std::endl;
}
