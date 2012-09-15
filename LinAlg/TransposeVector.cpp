#include "pch.h"

#include "TransposeVector.h"


namespace LinAlg_NS {

TransposeVector::TransposeVector(size_type dim)
    :
    vector_(Vector(dim)) {}

TransposeVector::TransposeVector(TransposeVector const & in)
    :
    vector_(in.vector_) {}

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

TransposeVector::TransposeVector(TransposeVector && in)
    :
    vector_(std::move(in.vector_)) {}

TransposeVector &
TransposeVector::operator=(TransposeVector && in) {
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
    vector_ = in.vector_;
}

double &
TransposeVector::operator()(size_type index) {
    return vector_(index);
}

double
TransposeVector::operator()(size_type index) const {
    return vector_(index);
}

TransposeVector::size_type
TransposeVector::size() const {
    return vector_.size();
}

} // namespace LinAlg_NS
