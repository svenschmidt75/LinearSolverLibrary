#include "pch.h"

#include "Vector.h"
#include "VectorMath.h"
#include "common/reporting.h"


namespace LinAlg_NS {

double
VectorMath::dotProduct(Vector const & lhs, Vector const & rhs) {
    bool cond = lhs.size() == rhs.size();

    BOOST_ASSERT_MSG(cond, "VectorMath::dotProduct: Size mismatch");
    if (!cond) {
        boost::format format = boost::format("VectorMath::dotProduct: Size mismatch\n");
        common_NS::reporting::error(format.str());
        throw std::runtime_error(format.str());
    }

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
