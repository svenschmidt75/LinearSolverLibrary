/*
* Name  : CoarseVariableSetIteratorLogic
* Path  : IIteratorLogic
* Use   : Implements the logic needed for an iterator over
          CoarseVariableSetDecorator.
* Author: Sven Schmidt
* Date  : 10/20/2013
*/
#pragma once

#include "DeclSpec.h"

#include "common/IIteratorLogic.h"
#include "IVariableSet.h"

#include <set>
#include <functional>

#include <boost/noncopyable.hpp>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS CoarseVariableSetIteratorLogic
        :
        public common_NS::IIteratorLogic<IVariableSet::size_type>,
        private boost::noncopyable {
    public:
        CoarseVariableSetIteratorLogic(IVariableSet const & variable_set, std::function<bool ()> filter);

        // FROM IIteratorLogic
        bool                    isValid() const;
        void                    next();
        IVariableSet::size_type get() const;

    private:
        IVariableSet const &   variable_set_;
        Iterator               it_;
        std::function<bool ()> filter_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
