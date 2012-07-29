#include "internal/ScalarExpression.h"

#include "Vector.h"


namespace LinAlg_NS {

namespace internal {

ScalarExpression::ScalarExpression(double value, Vector::size_type dim)
    :
    value_(value),
    dim_(dim) {}

ScalarExpression::ScalarExpression(ScalarExpression const & in)
    :
    value_(in.value_),
    dim_(in.dim_) {}

Vector::size_type
ScalarExpression::size() const {
    return dim_;
}

double
ScalarExpression::operator()(Vector::size_type /*index*/) const {
    return value_;
}

} // namespace internal

} // namespace LinAlg_NS
