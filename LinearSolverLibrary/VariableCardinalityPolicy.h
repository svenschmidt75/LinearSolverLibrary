/*
* Name  : VariableCardinalityPolicy
* Path  :
* Use   : Computes the cardinality for each undefined variable.
          see Trottenberg, page 474, formula for \lambda_{i}
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "VariableCategorizer.h"
#include "IVariableInfluenceAccessor.h"

#include "LinAlg/IMatrix2D.h"

#include <queue>

#include <boost/noncopyable.hpp>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS VariableCardinalityPolicy : private boost::noncopyable {
    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        VariableCardinalityPolicy(IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer const & categorizer);

    private:
        // node index, priority
        typedef std::pair<size_type, size_type> QueueItem_t;

    private:
        void initializeCardinality();

    private:
        IVariableInfluenceAccessor const & variable_influence_accessor_;
            VariableCategorizer const &      categorizer_;
        std::priority_queue<QueueItem_t> queue_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
