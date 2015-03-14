/*
 * Name  : AMGStrengthPolicyImpl
 * Path  : IAMGStrengthPolicy
 * Use   : 
 * Author: 
 * Date  : 03/14/2015
 */
#pragma once

#include "IAMGStrengthPolicy.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {


    class AMGStrengthPolicyImpl : public IAMGStrengthPolicy {
    public:
        std::unique_ptr<IVariableSet> getNeighborhood(LinAlg_NS::IMatrix2D::size_type variable) const;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
