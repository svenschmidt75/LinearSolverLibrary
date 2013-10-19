/*
* Name  : VariableInfluenceAccessor
* Path  :
* Use   : Adapter, returns S_i^{T} intersects fine and undefined variables,
          see Trottenberg, page 474
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "IVariableInfluenceAccessor.h"
#include "IVariableSet.h"
#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableCategorizer.h"

#include <boost/noncopyable.hpp>


namespace LinearSolverLibrary_NS {

    class VariableInfluenceAccessor : public IVariableInfluenceAccessor, private boost::noncopyable {
    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        VariableInfluenceAccessor(AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & categorizer);

        // FROM VariableInfluenceAccessor
        std::unique_ptr<IVariableSet> GetVariableInfluencedUndefined(size_type variable) const;
        std::unique_ptr<IVariableSet> GetVariableInfluencedFine(size_type variable) const;

    private:
        AMGStandardCoarseningStrengthPolicy const & strength_policy_;
        VariableCategorizer const &                 categorizer_;
    };

} // LinearSolverLibrary_NS

