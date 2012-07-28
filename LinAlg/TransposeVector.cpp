#include "TransposeVector.h"

#include <utility>
#include <stdexcept>

#include <boost/assert.hpp>


namespace LinAlg_NS {

TransposeVector::TransposeVector(size_type dim)
    :
    dim_(dim) {
    data_.resize(dim);
}

TransposeVector::TransposeVector(TransposeVector const & in)
    :
    dim_(in.dim_),
    data_(in.data_) {}

TransposeVector &
TransposeVector::operator=(TransposeVector const & in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // copy-construction is exception safe
    TransposeVector temp(in);

    swap(temp);

    return *this;
}

TransposeVector::TransposeVector(TransposeVector const && in)
    :
    data_(std::move(in.data_)) {}

TransposeVector &
TransposeVector::operator=(TransposeVector const && in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // force move copy-construction, is exception safe
    TransposeVector temp(std::move(in));

    swap(temp);

    return *this;
}

void
TransposeVector::swap(TransposeVector const & in) {
    data_ = in.data_;
}

double &
TransposeVector::operator()(size_type index) {
    bool assert_cond = index < data_.size();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("TransposeVector::operator(): Out of range error");

    return data_[index];
}

double
TransposeVector::operator()(size_type index) const {
    bool assert_cond = index < data_.size();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("TransposeVector::operator(): Out of range error");

    return data_[index];
}

TransposeVector::size_type
TransposeVector::size() const {
    return dim_;
}

} // namespace LinAlg_NS
