#include "pch.h"

#include "ScalarExpression.h"
#include "Vector.h"


namespace LinAlg_NS {

namespace internal {

ScalarExpression::ScalarExpression(double value)
    :
    value_(value) {}

ScalarExpression::ScalarExpression(ScalarExpression const & in)
    :
    value_(in.value_) {}

double
ScalarExpression::operator()(Vector::size_type /*index*/) const {
    return value_;
}

} // namespace internal

} // namespace LinAlg_NS
