#include "Vector.h"

#include <utility>
#include <stdexcept>

#include <boost/assert.hpp>


namespace LinAlg_NS {

Vector::Vector(size_type dim)
    :
    dim_(dim) {
    data_.resize(dim);
}

Vector::Vector(Vector const & in)
    :
    dim_(in.dim_),
    data_(in.data_) {}

Vector &
Vector::operator=(Vector const & in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // copy-construction is exception safe
    Vector temp(in);

    swap(temp);

    return *this;
}

Vector::Vector(Vector const && in)
    :
    data_(std::move(in.data_)) {}

Vector &
Vector::operator=(Vector const && in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // force move copy-construction, is exception safe
    Vector temp(std::move(in));

    swap(temp);

    return *this;
}

void
Vector::swap(Vector const & in) {
    data_ = in.data_;
}

double &
Vector::operator()(size_type index) {
    bool assert_cond = index < data_.size();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("Vector::operator(): Out of range error");

    return data_[index];
}

double
Vector::operator()(size_type index) const {
    bool assert_cond = index < data_.size();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("Vector::operator(): Out of range error");

    return data_[index];
}

Vector::size_type
Vector::size() const {
    return dim_;
}

} // namespace LinAlg_NS
