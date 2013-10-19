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
#include "LinAlg/IMatrix2D.h"

#include <set>

#include <boost/noncopyable.hpp>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS CoarseVariableSetDecorator : public IVariableSet, private boost::noncopyable {
    public:
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        // FROM IVariableSet
        bool      contains(size_type variable) const;
        size_type size() const;

    private:
        IVariableSet const & data_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
