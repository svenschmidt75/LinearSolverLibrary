/*
* Name  : CoarseVariableSetDecorator
* Path  :
* Use   : Decorates a VariableSet and filters the coarse grid indices.
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "IVariableSet.h"
#include "VariableCategorizer.h"
#include "LinAlg/IMatrix2D.h"

#include <set>

#include <boost/noncopyable.hpp>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS CoarseVariableSetDecorator final : public IVariableSet, private boost::noncopyable {

        // for iterator support
        friend class CoarseVariableSetIteratorLogic;


    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        CoarseVariableSetDecorator(IVariableSet const & variable_set, VariableCategorizer const & categorizer);

        // FROM IVariableSet
        bool                           contains(size_type variable) const override;
        size_type                      size() const override;
        common_NS::InputIterator<size_type> begin() const override;
        common_NS::InputIterator<size_type> end() const override;

    private:
        IVariableSet const &        variable_set_;
        VariableCategorizer const & categorizer_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)