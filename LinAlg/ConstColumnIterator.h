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
#include "ConstColumnIteratorBase.h"


namespace LinAlg_NS {


template<>
class LINALG_DECL_SYMBOLS ConstColumnIterator<SparseMatrix2D> {
public:
    typedef SparseMatrix2D::size_type size_type;

private:
    typedef ConstColumnIterator<SparseMatrix2D> iter;

public:
    ConstColumnIterator(SparseMatrix2D const & m, size_type row);

//    explicit bool operator();

    size_type column() const;

    double operator++() const;
    double operator++(int) const;

    double operator*() const;

private:
    void jumpToFirstElement() const;
    void jumpToNextElement() const;

private:
    SparseMatrix2D    m_;
    mutable size_type column_;
    mutable size_type column_mapped_;
    size_type         row_;
};


} // namespace LinAlg_NS
