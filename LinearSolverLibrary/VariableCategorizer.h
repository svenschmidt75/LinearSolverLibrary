/*
* Name  : VariableCategorizer
* Path  :
* Use   : Categorizes a grid node into
          - undefined
          - coarse
          - fine
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "IVariableSet.h"
#include "LinAlg/IMatrix2D.h"

#include <set>

#include <boost/noncopyable.hpp>


namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS VariableCategorizer {
    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

        enum class Type {UNDEFINED, COARSE, FINE};

    public:
    };

} // LinearSolverLibrary_NS
