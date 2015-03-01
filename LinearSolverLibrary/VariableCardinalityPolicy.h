/*
* Name  : VariableCardinalityPolicy
* Path  : 
* Use   : Computes the cardinality for each undefined variable.
          see Trottenberg, page 474, formula for \lambda_{i}.
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class IVariableInfluenceAccessor;


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS VariableCardinalityPolicy {
    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        VariableCardinalityPolicy(IVariableInfluenceAccessor const & variable_influence_accessor);
        VariableCardinalityPolicy(VariableCardinalityPolicy const &) = delete;
        VariableCardinalityPolicy & operator=(VariableCardinalityPolicy const &) = delete;

        size_t GetCardinalityForVariable(size_type variable) const;

    private:
        IVariableInfluenceAccessor const & variable_influence_accessor_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
