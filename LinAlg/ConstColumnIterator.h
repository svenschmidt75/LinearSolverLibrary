/*
* Name  : ConstColumnIterator
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once


#include "DeclSpec.h"
#include "SparseMatrix2D.h"
#include "ConstColumnIteratorBase.h"
#include "ConstRowIteratorBase.h"


namespace LinAlg_NS {


template<>
class LINALG_DECL_SYMBOLS ConstColumnIterator<SparseMatrix2D> {
public:
    typedef SparseMatrix2D::size_type size_type;

private:
    typedef ConstColumnIterator<SparseMatrix2D> iter;

public:
    explicit ConstColumnIterator(SparseMatrix2D const & m);
    explicit ConstColumnIterator(SparseMatrix2D const & m, size_type startColumn);

    size_type maxColumns() const;
    size_type column() const;

    iter const & operator++() const;
    iter operator++(int) const;
        
    ConstRowIterator<SparseMatrix2D> operator*() const;

private:
    SparseMatrix2D    m_;
    mutable size_type current_column_;
};


} // namespace LinAlg_NS
