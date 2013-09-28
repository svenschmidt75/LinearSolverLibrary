/*
* Name  : iterators
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 09/22/2013
*/
#pragma once


#include "DeclSpec.h"
#include "SparseMatrix2D.h"
#include "ConstColumnIterator.h"
#include "ConstRowIterator.h"


namespace LinAlg_NS {


class LINALG_DECL_SYMBOLS iterators {
public:
    static ConstColumnIterator<SparseMatrix2D> getConstIterator(SparseMatrix2D const & matrix);

};

} // namespace LinAlg_NS
