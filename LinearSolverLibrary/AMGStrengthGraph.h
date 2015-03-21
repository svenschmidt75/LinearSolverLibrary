/*
 * Name  : AMGStrengthGraph.h
 * Path  :
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/21/2015
 */
#pragma once

#include "DeclSpec.h"

#include "AMGStrengthPolicyImpl.h"


 // forward declarations
namespace LinAlg_NS {
    class SparseMatrix2D;
}


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

    class DECL_SYMBOLS AMGStrengthGraph : public AMGStrengthPolicyImpl {
    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        explicit AMGStrengthGraph(AMGStrengthPolicyImpl const & strength_policy);
        AMGStrengthGraph(AMGStrengthGraph const &) = delete;
        AMGStrengthGraph & operator=(AMGStrengthGraph const &) = delete;

        bool      hasEdge(size_type v1, size_type v2) const;
        void      removeEdge(size_type v1, size_type v2);
        size_type nEdgesRemoved(size_type v) const;
        bool      hasEdges(size_type v) const;

    private:
        using IndexMapping_t = std::map<size_type, std::set<size_type>>;

    private:
        AMGStrengthPolicyImpl const & strength_policy_;
        IndexMapping_t                removed_edges_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
