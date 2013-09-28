/*
* Name  : ConstRowIterator
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/28/2013
*/
#pragma once


#include "DeclSpec.h"
#include "SparseMatrix2D.h"
#include "ConstRowIteratorBase.h"


namespace LinAlg_NS {


template<>
class LINALG_DECL_SYMBOLS ConstRowIterator<SparseMatrix2D> {
public:
    typedef SparseMatrix2D::size_type size_type;

private:
    typedef ConstRowIterator<SparseMatrix2D> iter;

public:
    ConstRowIterator(SparseMatrix2D const & m, size_type column, size_type row);

private:
    SparseMatrix2D    m_;
    mutable size_type column_;
    mutable size_type row_;
};


} // namespace LinAlg_NS
