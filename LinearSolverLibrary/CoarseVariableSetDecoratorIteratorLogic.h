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

#include "IVariableSet.h"
#include "common/IInputIteratorLogic.h"
#include "common/InputIterator.hpp"

#include <set>
#include <functional>

#include <boost/noncopyable.hpp>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS CoarseVariableSetDecoratorIteratorLogic
        :
        public common_NS::IInputIteratorLogic<IVariableSet::size_type>,
        private boost::noncopyable {
    public:
        typedef IVariableSet::size_type size_type;
        typedef IInputIteratorLogic<size_type> This_t;

    public:
        CoarseVariableSetDecoratorIteratorLogic(common_NS::InputIterator<IVariableSet::size_type> begin_it_, common_NS::InputIterator<IVariableSet::size_type> end_it_, std::function<bool (IVariableSet::size_type)> filter);

        // FROM IInputIteratorLogic
        size_type               get() const override;
        void                    next() override;
        bool                    equal(IInputIteratorLogic const & in) const override;
        std::unique_ptr<This_t> clone() const override;

    private:
        common_NS::InputIterator<IVariableSet::size_type>  iterator_;
        common_NS::InputIterator<IVariableSet::size_type>  end_iterator_;
        std::function<bool (IVariableSet::size_type)>      filter_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
