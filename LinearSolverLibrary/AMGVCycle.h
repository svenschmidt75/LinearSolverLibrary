/*
* Name  : AMGVCycle
* Path  : AMGVWCycleBase
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "DeclSpec.h"

#include "AMGVWCycleBase.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGVCycle : public AMGVWCycleBase {
    public:
        AMGVCycle();
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
