/*
 * Name  : AMGStandardStrengthPolicy
 * Path  : IAMGStrengthPolicy
 * Use   : Determines that strong connections of a variable on others, i.e.
           S_{i} and S_{i}^{T}, see Trottenberg, page 473, 474.
           The strength variable i is connected to variables j is measured
           based on the smallest negative value a_{ij} * eps.
 * Author: Sven Schmidt
 * Date  : 10/12/2013
 */
#pragma once

#include "DeclSpec.h"

#include "AMGStrengthPolicyImpl.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IVariableSet;
}


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

    class DECL_SYMBOLS AMGStandardStrengthPolicy : public AMGStrengthPolicyImpl {
    public:
        explicit AMGStandardStrengthPolicy(LinAlg_NS::SparseMatrix2D const & m);
        AMGStandardStrengthPolicy(AMGStandardStrengthPolicy const &) = delete;
        AMGStandardStrengthPolicy & operator=(AMGStandardStrengthPolicy const &) = delete;

        // FROM IAMGStrengthPolicy
        std::unique_ptr<IVariableSet> getStrongInfluencers(LinAlg_NS::IMatrix2D::size_type variable) const override;
        std::unique_ptr<IVariableSet> getStronglyInfluenced(LinAlg_NS::IMatrix2D::size_type variable) const override;
        void                          exportToGraphviz() const override;

    private:
        using Graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>;

    private:
        void computeConnections(LinAlg_NS::SparseMatrix2D const & m);
        void computeConnectionsForVariable(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::IMatrix2D::size_type i, double max_element);

    private:
        /* The graph contains edge e(i, j) if node i strongly depends on node j.
           see Trottenberg, p. 473.
        */
        Graph_t      g_;
        double const eps_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
