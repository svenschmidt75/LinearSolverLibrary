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

#include "common/Iterator.hpp"
#include "common/IIteratorLogic.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS VariableSetIteratorLogic : public common_NS::IIteratorLogic<IVariableSet::size_type> {
    public:
        VariableSetIteratorLogic(VariableSet const & variable_set);

        VariableSetIteratorLogic & operator=(VariableSetIteratorLogic const & in);

        // FROM IIteratorLogic
        bool isValid() const;
        void next();
        IVariableSet::size_type get() const;
        std::unique_ptr<IIteratorLogic<IVariableSet::size_type>> end() const;
        bool equalTo(IIteratorLogic<IVariableSet::size_type> const & in) const;

    private:
        VariableSet const &                variable_set_;
        VariableSet::Set_t::const_iterator it_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
