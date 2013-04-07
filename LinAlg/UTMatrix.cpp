#include "pch.h"

#include "UTMatrix.h"


namespace LinAlg_NS {


UTMatrix::UTMatrix()
    :
    ncols_(0) {}

UTMatrix::UTMatrix(size_type ncols)
    :
    ncols_(ncols) {

    data_.resize(ncols);
}

UTMatrix::UTMatrix(UTMatrix const & in)
    :
    ncols_(in.ncols_),
    data_(in.data_) {}

UTMatrix &
UTMatrix::operator=(UTMatrix const & in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // copy-construction is exception safe
    UTMatrix temp(in);

    swap(temp);

    return *this;
}

UTMatrix::UTMatrix(UTMatrix && in)
    :
    ncols_(in.ncols_),              // value semantics: no move necessary
    data_(std::move(in.data_)) {}

UTMatrix &
UTMatrix::operator=(UTMatrix && in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // force move copy-construction, is exception safe
    UTMatrix temp(std::move(in));

    swap(temp);

    return *this;
}

void
UTMatrix::swap(UTMatrix const & in) {
    // to enable the safe exception guarantee
    ncols_     = in.ncols_;
    data_      = in.data_;
}

UTMatrix::size_type
UTMatrix::rows() const {
    return ncols_;
}

UTMatrix::size_type
UTMatrix::cols() const {
    return ncols_;
}

namespace {

    UTMatrix::size_type
    index(UTMatrix::size_type i, UTMatrix::size_type j) {
        return j * (j + 1) / 2 + i;
    }
}

double
UTMatrix::operator()(UTMatrix::size_type i, UTMatrix::size_type j) const {
    // i: x, j: y
    bool assert_cond = i < ncols_ && j < ncols_;
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("UTMatrix::operator(): Out of range error");

    if (i > j)
        return 0;

    return data_[index(i, j)];
}

double &
UTMatrix::operator()(UTMatrix::size_type i, UTMatrix::size_type j) {
    // i: x, j: y
    bool assert_cond = i < ncols_ && j < ncols_ && i <= j;
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("UTMatrix::operator(): Out of range error");

    return data_[index(i, j)];
}

void
UTMatrix::print() const {
    for (size_type row = 0; row < ncols_; ++row) {
        for (size_type col = 0; col < row; ++col) {
            double value = data_[index(row, col)];
            std::cout << std::setw(8) << 0 << value;
        }    
    }    

    std::cout << std::endl;
}

} // namespace LinAlg_NS
