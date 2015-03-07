/*
 * Name  : AMGStandardCoarsening
 * Path  : 
 * Use   : Splits nodes into coarse and fine.
 * Author: Sven Schmidt
 * Date  : 10/19/2013
 */
#pragma once

#include "DeclSpec.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IVariableInfluenceAccessor;
    class VariableCategorizer;
}


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardCoarsening {
    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        AMGStandardCoarsening(LinAlg_NS::SparseMatrix2D const & m_,
                             IVariableInfluenceAccessor const & variable_influence_accessor,
                             VariableCategorizer & categorizer);

        AMGStandardCoarsening(AMGStandardCoarsening const &) = delete;
        AMGStandardCoarsening & operator=(AMGStandardCoarsening const &) = delete;

        void coarsen();

    private:
        // node index, priority
        using QueueItem_t = std::pair<size_type, size_type>;
        using Queue_t = std::priority_queue<QueueItem_t>;

    private:
        Queue_t initializeVariableCardinality();
        void    categorizeVariablesStronglyInfluencing(LinAlg_NS::IMatrix2D::size_type variable);
        void    adjustCardinalityOfStrongInfluencers(std::set<LinAlg_NS::IMatrix2D::size_type> const & variable);

    private:
        LinAlg_NS::SparseMatrix2D const &  m_;
        IVariableInfluenceAccessor const & variable_influence_accessor_;
        VariableCategorizer &              categorizer_;
        Queue_t                            queue_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
