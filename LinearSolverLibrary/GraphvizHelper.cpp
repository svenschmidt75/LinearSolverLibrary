#include "pch.h"

#include "GraphvizHelper.h"

#include "IAMGStrengthPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
GraphvizHelper::exportToGraphviz(IMatrix2D::size_type nvertices, IAMGStrengthPolicy const & strength_policy) {
    std::ofstream stream(R"(D:\Develop\Ideas\LinearSolverLibrary\graph.log)");
    stream << std::endl;
    stream << std::endl;
    stream << "digraph G {" << std::endl;
    for (IMatrix2D::size_type i = 0; i < nvertices; ++i) {
        auto vertices = strength_policy.getStrongInfluencers(i);
        for (auto target_index : *vertices)
            stream << "  " << i << " -> " << target_index << std::endl;
    }
    stream << "}" << std::endl;
    stream << std::endl;
}
