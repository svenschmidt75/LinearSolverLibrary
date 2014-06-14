/*
* Name  : UndefinedVariableSetDecorator
* Path  : VariableSetDecoratorBase
* Use   : Decorates a VariableSet and filters the undefined grid indices.
* Author: Sven Schmidt
* Date  : 10/27/2013
*/
#pragma once

#include "DeclSpec.h"

#include "VariableSetDecoratorBase.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    // forward declarations
    class IVariableCategorizer;
    class IVariableSet;


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS UndefinedVariableSetDecorator final : public VariableSetDecoratorBase {
    public:
        UndefinedVariableSetDecorator(std::shared_ptr<IVariableSet> const & variable_set, IVariableCategorizer const & categorizer);

    protected:
        std::function<bool(size_type)> predicate() const override;

    private:
        IVariableCategorizer const & categorizer_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
