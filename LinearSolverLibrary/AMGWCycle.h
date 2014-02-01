/*
* Name  : AMGWCycle
* Path  : AMGVWCycleBase
* Use   : 
* Author: Sven Schmidt
* Date  : 01/19/2014
*/
#pragma once

#include "DeclSpec.h"

#include "AMGVWCycleBase.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGWCycle : public AMGVWCycleBase {
    public:
        AMGWCycle(short gamma);
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
