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

#include <vector>

#include <boost/noncopyable.hpp>


#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS VariableCategorizer {
    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

        enum class Type : char {UNDEFINED = 'U', COARSE = 'C', FINE = 'F'};

    public:
        VariableCategorizer(size_type numberOfVariables);

        void initializeVariablesToUndefined();

    private:
        typedef std::vector<Type> VariableData_t;

    private:
        VariableData_t data_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
