/*
 * Name  : Helper
 * Path  : 
 * Use   : Implements helper functions for objects defined
 *         in LinAlg.
 * Author: Sven Schmidt
 * Date  : 07/29/2011
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"


namespace LinAlg_NS {

    class Vector;
    class TransposeVector;
    class Matrix2D;
    class SparseMatrix2D;


    class DECL_SYMBOLS helper {
    public:
        static TransposeVector transpose(Vector const & vec);
        static Vector          transpose(TransposeVector const & vec);
        static double          get_value(Matrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);
        static double          get_value(SparseMatrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);
    };

} // namespace LinAlg_NS
