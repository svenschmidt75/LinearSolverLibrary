/*
* Name  : VariableSetIteratorLogic
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "VariableSet.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

    class DECL_SYMBOLS VariableSetIteratorLogic
        : public common_NS::IInputIteratorLogic<IVariableSet::size_type> {
    public:
        typedef IVariableSet::size_type size_type;
        typedef IInputIteratorLogic<size_type> This_t;

    public:
        VariableSetIteratorLogic(VariableSet::DataIterator_t iterator);

        // FROM IInputIteratorLogic
        size_type               get() const override;
        void                    next() override;
        bool                    equal(IInputIteratorLogic const & in) const override;
        std::unique_ptr<This_t> clone() const override;

    private:
        VariableSet::DataIterator_t iterator_;

    private:
        VariableSetIteratorLogic(VariableSetIteratorLogic const &) = default;
        VariableSetIteratorLogic & operator=(VariableSetIteratorLogic const &) = default;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
