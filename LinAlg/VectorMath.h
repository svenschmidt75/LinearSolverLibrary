/*
 * Name  : math
 * Path  : 
 * Use   : Implements various conjugate gradient methods
 * Author: Sven Schmidt
 * Date  : 23/11/2012
 */
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {

    class Vector;


    class LINALG_DECL_SYMBOLS VectorMath {
    public:
        static double dotProduct(Vector const & lhs, Vector const & rhs);
        static double norm(Vector const & v);
    };

} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
