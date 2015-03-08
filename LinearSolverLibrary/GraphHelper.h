 /*
  * Name  : GraphHelper
  * Path  :
  * Use   : Provides the means to remove an edge from a strength matrix
  * Author: Sven Schmidt
  * Date  : 03/08/2015
  */
 #pragma once


#include "DeclSpec.h"


  // forward declarations
namespace LinAlg_NS {
    class SparseMatrix2D;
}


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

    
    class DECL_SYMBOLS GraphHelper { 
    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        explicit GraphHelper(IAMGStandardStrengthPolicy const & strength_policy);
        GraphHelper(GraphHelper const &) = delete;
        GraphHelper & operator=(GraphHelper const &) = delete;

        bool hasEdge(size_type v1, size_type v2) const;
        void removeEdge(size_type v1, size_type v2);

    private:
        using IndexMapping_t = std::map<size_type, std::set<size_type>>;

    private:
        IAMGStandardStrengthPolicy const & strength_policy_;
        IndexMapping_t                     removed_edges_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
