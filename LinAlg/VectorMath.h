/*
 * Name  : math
 * Path  : 
 * Use   : Implements common methods on vectors.
 * Author: Sven Schmidt
 * Date  : 11/23/2012
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
        static double dotProductSerial(Vector const & lhs, Vector const & rhs);
        static double chunkedParallelDotProduct(Vector const & v1, Vector const & v2);
        static double nonChunkedParallelDotProduct(Vector const & v1, Vector const & v2);

        static double norm(Vector const & v);

        static double LinfError(Vector const & lhs, Vector const & rhs);
        static double L2Error(Vector const & lhs, Vector const & rhs);
    };

} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
