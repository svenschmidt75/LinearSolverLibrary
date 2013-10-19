/*
* Name  : IVariableSet
* Path  :
* Use   : Base class containing sets of "grid" indices
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "LinAlg/IMatrix2D.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS IVariableSet {
    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        virtual bool      contains(size_type variable) const = 0;
        virtual size_type size() const = 0;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
