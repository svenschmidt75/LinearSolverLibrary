/*
* Name  : IVariableInfluenceAccessor
* Path  :
* Use   : Returns S_i^{T}, see Trottenberg, page 474.
          This is an adapter class, that allows to remove an explicit  
          dependence on a specific connection strength policy.  
          For example, VariableCardinalityPolicy accesses the connections  
          via this class, not via direct dependence on AMGStandardCoarseningStrengthPolicy.  
          An alternative to using an explicit adapter is to have each connection strength
          policy inherit from this class and implements its interface. But this requires a
          lot of multiple inheritance (of interfaces though)...
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "IVariableSet.h"


namespace LinearSolverLibrary_NS {

    class IVariableInfluenceAccessor {
    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        virtual std::unique_ptr<IVariableSet> GetVariableInfluencedUndefined(size_type variable) const = 0;
        virtual std::unique_ptr<IVariableSet> GetVariableInfluencedFine(size_type variable) const = 0;
    };

} // LinearSolverLibrary_NS
