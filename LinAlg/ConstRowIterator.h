/*
* Name  : ConstRowIterator
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once


#include "DeclSpec.h"
#include "SparseMatrix2D.h"
#include "ConstRowIteratorBase.h"
#include "ConstColumnIteratorBase.h"


namespace LinAlg_NS {


template<>
class LINALG_DECL_SYMBOLS ConstRowIterator<SparseMatrix2D> {
public:
    typedef SparseMatrix2D::size_type size_type;

private:
    typedef ConstRowIterator<SparseMatrix2D> iter;

public:
    explicit ConstRowIterator(SparseMatrix2D const & m);
    explicit ConstRowIterator(SparseMatrix2D const & m, size_type row);

    bool next() const;

    size_type maxRows() const;
    size_type row() const;
    size_type numberOfNonZeroMatrixElements() const;

    iter const & operator++() const;
    iter operator++(int) const;
        
    ConstColumnIterator<SparseMatrix2D> operator*() const;

private:
    SparseMatrix2D    m_;
    mutable size_type row_;
};


} // namespace LinAlg_NS
