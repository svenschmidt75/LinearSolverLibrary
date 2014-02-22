/*
* Name  : AMGCoarseVariableIndexer
* Path  :
* Use   : Maps coarse variables into sequential indices
*         for use in the interpolation operator calculation.
* Author: Sven Schmidt
* Date  : 02/22/2014
*/
#pragma once

#include "DeclSpec.h"


#include "VariableCategorizer.h"


// #pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGCoarseVariableIndexer {
    private:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        AMGCoarseVariableIndexer(VariableCategorizer const & categorizer);

        size_type Index(size_type variable_index) const;
        size_type NumberOfVariables() const;

    private:
        std::map<size_type, size_type> map_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
// #pragma warning(default:4275)