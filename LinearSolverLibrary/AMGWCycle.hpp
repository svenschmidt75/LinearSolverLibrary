/*
* Name  : AMGWCycle
* Path  : AMGVWCycleBase
* Use   : 
* Author: Sven Schmidt
* Date  : 01/19/2014
*/
#pragma once

#include "DeclSpec.h"

#include "AMGVWCycleBase.hpp"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    // GAMMA indicates how many peaks theW cycle has (minus the
    // 1st and the last), i.e. W has one peak (in the middle).

    template<int GAMMA>
    class AMGWCycle : public AMGVWCycleBase<GAMMA> {
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
