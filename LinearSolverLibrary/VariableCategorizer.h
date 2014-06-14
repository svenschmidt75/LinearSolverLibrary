/*
 * Name  : VariableCategorizer
 * Path  : IVariableCategorizer
 * Use   : Categorizes a grid node into
           - undefined
           - coarse
           - fine
 * Author: Sven Schmidt
 * Date  : 10/19/2013
 */
#pragma once

#include "IVariableCategorizer.h"

#include "DeclSpec.h"


#pragma warning(disable:4251)
#pragma warning(disable:4275)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS VariableCategorizer : public IVariableCategorizer {
    public:
        VariableCategorizer(size_type numberOfVariables);

        Type      GetType(size_type variable) const override;
        void      SetType(size_type variable, Type type) override;
        size_type NumberOfVariables() const override;
        void      print() const override;

    private:
        void initializeVariablesToUndefined();

    private:
        typedef std::vector<Type> VariableData_t;

    private:
        VariableData_t data_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
