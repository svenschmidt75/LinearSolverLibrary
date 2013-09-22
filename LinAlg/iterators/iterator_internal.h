/*
* Name  : iterator_internal
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once


#include "..\DeclSpec.h"
#include "..\SparseMatrix2D.h"


namespace LinAlg_NS {

namespace iterator_internal {

    template<typename T>
    class ConstColumnIterator;

    template<>
    class LINALG_DECL_SYMBOLS ConstColumnIterator<SparseMatrix2D> {
    public:
        typedef SparseMatrix2D::size_type size_type;

    public:
        explicit ConstColumnIterator(SparseMatrix2D const & m);
        size_type size() const;

    private:
        SparseMatrix2D m_;
    };

} // namespace iterator_internal

} // namespace LinAlg_NS
