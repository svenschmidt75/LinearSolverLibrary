#include "pch.h"

#include "UHMatrix.h"


namespace LinAlg_NS {


UHMatrix::UHMatrix() {}

UHMatrix::UHMatrix(size_type ncols)
    :
    ncols_(ncols) {

    // treat as upper-triangular with cols + 1
    ncols++;
    data_.resize(ncols * (ncols + 1) / 2);
}

UHMatrix::UHMatrix(UHMatrix const & in)
    :
    ncols_(in.ncols_),
    data_(in.data_) {}

UHMatrix &
UHMatrix::operator=(UHMatrix const & in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // copy-construction is exception safe
    UHMatrix temp(in);

    swap(temp);

    return *this;
}

UHMatrix::UHMatrix(UHMatrix && in)
    :
    ncols_(in.ncols_),              // value semantics: no move necessary
    data_(std::move(in.data_)) {}

UHMatrix &
UHMatrix::operator=(UHMatrix && in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // force move copy-construction, is exception safe
    UHMatrix temp(std::move(in));

    swap(temp);

    return *this;
}

void
UHMatrix::swap(UHMatrix const & in) {
    // to enable the safe exception guarantee
    ncols_ = in.ncols_;
    data_  = in.data_;
}

UHMatrix::size_type
UHMatrix::rows() const {
    return ncols_ + 1;
}

UHMatrix::size_type
UHMatrix::cols() const {
    return ncols_;
}

namespace {

    UHMatrix::size_type
    index(UHMatrix::size_type i, UHMatrix::size_type j) {
        return j * (j + 1) / 2 + i;
    }
}

double
UHMatrix::operator()(UHMatrix::size_type i, UHMatrix::size_type j) const {
    // i: x, j: y
    bool assert_cond = i < rows() && j < cols();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("UHMatrix::operator(): Out of range error");

    if (i > j + 1)
        return 0;

    return data_[index(i, j)];
}

double &
UHMatrix::operator()(UHMatrix::size_type i, UHMatrix::size_type j) {
    // i: x, j: y
    bool assert_cond = i < rows() && j < cols() && i <= j + 1;
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("UHMatrix::operator(): Out of range error");

    return data_[index(i, j)];
}

void
UHMatrix::print() const {
    std::cout << std::endl;

    for (size_type row = 0; row < ncols_ + 1; ++row) {
        for (size_type col = 0; col < ncols_; ++col) {
            if (row > col + 1) {
                std::cout << boost::format("%5s") % " ";
                continue;
            }

            double value = data_[index(row, col)];
            std::cout << boost::format("%5d") % value;
        }

        std::cout << std::endl;
    }    

    std::cout << std::endl;
}

} // namespace LinAlg_NS
