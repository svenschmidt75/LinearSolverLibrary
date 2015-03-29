/*
* Name  : CoarseVariableSetDecorator
* Path  : VariableSetDecoratorBase
* Use   : Decorates a VariableSet and filters the coarse grid indices.
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "VariableSetDecoratorBase.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class VariableCategorizer;
    class IVariableSet;


    class LINEAR_SOLVER_LIBRARY_DECL_SYMBOLS CoarseVariableSetDecorator final : public VariableSetDecoratorBase {
    public:
        CoarseVariableSetDecorator(std::shared_ptr<IVariableSet> const & variable_set, VariableCategorizer const & categorizer);

    protected:
        std::function<bool(size_type)> predicate() const override;

    private:
        VariableCategorizer const & categorizer_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
