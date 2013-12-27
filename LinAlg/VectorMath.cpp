#include "pch.h"

#include "Vector.h"
#include "VectorMath.h"


namespace LinAlg_NS {

double
VectorMath::dotProduct(Vector const & lhs, Vector const & rhs) {
#ifdef _DEBUG
    common_NS::reporting::checkConditional(lhs.size() == rhs.size(), "VectorMath::dotProduct: Size mismatch");
#endif
    double tmp = 0.0;
    for (Vector::size_type i = 0; i < lhs.size(); ++i) {
        tmp += lhs(i) * rhs(i);
    }
    return tmp;
}

double
VectorMath::norm(Vector const & v) {
    return std::sqrt(dotProduct(v, v));
}

} // namespace LinAlg_NS
