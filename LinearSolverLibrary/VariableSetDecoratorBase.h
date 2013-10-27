/*
* Name  : VariableSetDecoratorBase
* Path  :
* Use   : Decorates a VariableSet and filters the fine grid indices.
* Author: Sven Schmidt
* Date  : 10/26/2013
*/
#pragma once

#include "DeclSpec.h"

#include "IVariableSet.h"
#include "common/InputIterator.hpp"

#include <functional>
#include <memory>

#include <boost/noncopyable.hpp>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS VariableSetDecoratorBase 
        :
        public IVariableSet,
        private boost::noncopyable {
    public:
        typedef IVariableSet::size_type size_type;
        typedef common_NS::InputIterator<size_type> InputIterator_t;

    public:
        VariableSetDecoratorBase(std::shared_ptr<IVariableSet> const & variable_set);

        // FROM IVariableSet
        bool            contains(size_type variable) const override;
        size_type       size() const override;
        InputIterator_t begin() const override;
        InputIterator_t end() const override;

    protected:
        virtual std::function<bool(size_type)> predicate() const = 0;

    private:
        std::shared_ptr<IVariableSet> variable_set_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
