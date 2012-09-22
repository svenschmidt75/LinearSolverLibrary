/*
 * Name  : MultiColor
 * Path  : 
 * Use   : Computes the multi-color decomposition into independent sets
 *         for a sparse matrix.
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include "pch.h"


namespace LinAlg_NS {
    class  SparseMatrix2D;
}


namespace internal {

    class MultiColor {
    public:
        static LinAlg_NS::SparseMatrix2D apply(LinAlg_NS::SparseMatrix2D const & m);
    };
}
