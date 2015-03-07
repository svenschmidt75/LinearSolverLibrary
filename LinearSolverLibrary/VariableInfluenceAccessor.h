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


// forward declarations
namespace LinearSolverLibrary_NS {
    class IAMGStandardStrengthPolicy;
    class AMGStandardStrengthPolicy;
    class VariableCategorizer;
}


#pragma warning(disable:4275)


namespace LinearSolverLibrary_NS {

    class DECL_SYMBOLS VariableInfluenceAccessor : public IVariableInfluenceAccessor {

    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        VariableInfluenceAccessor(IAMGStandardStrengthPolicy const & strength_policy, VariableCategorizer const & categorizer);

        VariableInfluenceAccessor(VariableInfluenceAccessor const &) = delete;
        VariableInfluenceAccessor & operator=(VariableInfluenceAccessor const &) = delete;

        // FROM VariableInfluenceAccessor
        std::unique_ptr<IVariableSet> GetVariableInfluencedUndefined(size_type variable) const override;
        std::unique_ptr<IVariableSet> GetVariableInfluencedFine(size_type variable) const override;

    private:
        IAMGStandardStrengthPolicy const & strength_policy_;
        VariableCategorizer const &        categorizer_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4275)
