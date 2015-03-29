/*
 * Name  : VariableSet
 * Path  : IVariableSet
 * Use   : Contains a set of "grid" indices
 * Author: Sven Schmidt
 * Date  : 10/13/2013
 */
#pragma once

#include "DeclSpec.h"

#include "IVariableSet.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEAR_SOLVER_LIBRARY_DECL_SYMBOLS VariableSet final : public IVariableSet {

        // for iterator support
        friend class VariableSetIteratorLogic;


    public:
        typedef IVariableSet::size_type size_type;
        typedef common_NS::InputIterator<size_type> InputIterator_t;

    public:
        // FROM IVariableSet
        bool            contains(size_type variable) const override;
        size_type       size() const override;
        InputIterator_t begin() const override;
        InputIterator_t end() const override;

        // LOCAL methods
        void            add(size_type variable);

    protected:
        typedef IVariableSet::Set_t::const_iterator DataIterator_t;

    private:
        IVariableSet::Set_t data_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
