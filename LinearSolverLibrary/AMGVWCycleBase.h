/*
* Name  : AMGVWCycleBase
* Path  : IAMGCycle
* Use   :
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "DeclSpec.h"

#include "IAMGCycle.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGVWCycleBase : public IAMGCycle {
    public:
        AMGVWCycleBase(short gamma);

        // FROM IAMGCycle
        void     build(short max_level) override;
        void     print() const override;
        iterator begin() const override;
        iterator end() const override;

    private:
        void generateLevel(int current_level) const;

    private:
        mutable Type_t levels_;
        mutable size_t levels_index_;
        mutable int    max_depth_;
        short          gamma_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
