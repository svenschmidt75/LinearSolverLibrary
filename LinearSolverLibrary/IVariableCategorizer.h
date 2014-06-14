/*
 * Name  : IVariableCategorizer
 * Path  :
 * Use   : Categorizes a grid node into
           - undefined
           - coarse
           - fine
 * Author: Sven Schmidt
 * Date  : 06/14/2014
 */
#pragma once


namespace LinearSolverLibrary_NS {

    class IVariableCategorizer {
    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

        enum class Type : char {UNDEFINED = 'U', COARSE = 'C', FINE = 'F'};

    public:
        ~IVariableCategorizer() {}

        virtual Type      GetType(size_type variable) const = 0;
        virtual void      SetType(size_type variable, Type type) = 0;
        virtual size_type NumberOfVariables() const = 0;
        virtual void      print() const = 0;
    };

} // LinearSolverLibrary_NS
