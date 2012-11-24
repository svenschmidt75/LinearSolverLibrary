#include "pch.h"

#include "Vector.h"

#include "VectorMath.h"


namespace LinAlg_NS {

double
VectorMath::dotProduct(Vector const & lhs, Vector const & rhs) {
    double tmp = lhs(0) * rhs(0) + lhs(1) * rhs(1) + lhs(2) * rhs(2);
    return tmp;
}

double
VectorMath::norm(Vector const & v) {
    return std::sqrt(dotProduct(v, v));
}

} // namespace LinAlg_NS
