/*
* Name  : VariableSet
* Path  :
* Use   : Contains a set of "grid" indices
* Author: Sven Schmidt
* Date  : 10/13/2013
*/
#pragma once

#include "DeclSpec.h"

#include "IVariableSet.h"
#include "LinAlg/IMatrix2D.h"

#include <set>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS VariableSet : public IVariableSet {
    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        // FROM IVariableSet
        bool      contains(size_type variable) const;
        size_type size() const;

        // LOCAL methods
        void      add(size_type variable);

    private:
        std::set<size_type> data_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
