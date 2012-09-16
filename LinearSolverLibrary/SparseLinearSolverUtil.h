/*
 * Name  : SparseLinearSolverUtil
 * Path  : 
 * Use   : Utility functions for various sparse linear solver codes.
           Note: This class is tightly coupled to the implementation
           of SparseMatrix2D.
 * Author: Sven Schmidt
 * Date  : 09/15/2012
 */
#pragma once

#include "DeclSpec.h"


namespace LinAlg_NS {
    class SparseMatrix2D;
    class Vector;
}


namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS SparseLinearSolverUtil {
    public:
        static bool isDiagonallyDominant(LinAlg_NS::SparseMatrix2D const & m);
        static bool isVectorEqual(LinAlg_NS::Vector const & v1, LinAlg_NS::Vector const & v2, double tol);
    };

} // LinearSolverLibrary_NS
