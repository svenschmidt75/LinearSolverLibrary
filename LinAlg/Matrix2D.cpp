#include "pch.h"

#include "Matrix2D.h"
#include "Vector.h"

namespace LinAlg_NS {


Matrix2D::Matrix2D(Matrix2D::size_type rows, Matrix2D::size_type cols)
    :
    rows_(rows),
    cols_(cols) {
    // Initialize with 0
    data_.resize(rows_ * cols_, 0);
}

Matrix2D::Matrix2D(Matrix2D const & in)
    :
    rows_(in.rows_),
    cols_(in.cols_),
    data_(in.data_) {}

Matrix2D &
Matrix2D::operator=(Matrix2D const & in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // force move copy-construction, is exception safe
    Matrix2D temp(in);
    swap(temp);
    return *this;
}

Matrix2D::Matrix2D(Matrix2D && in)
    :
    rows_{in.rows_},
    cols_{in.cols_},
    data_(std::move(in.data_)) {}
    
Matrix2D &
Matrix2D::operator=(Matrix2D && in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // force move copy-construction, is exception safe
    Matrix2D temp(std::move(in));
    swap(temp);
    return *this;
}

Matrix2D &
Matrix2D::operator<<(double value) {
    // The ordering is such that the upper left corner is the origin.

    // free memory allocated in constructor
    data_.resize(0);
    common_NS::reporting::checkUppderBound(data_.size(), rows_ * cols_ - 1);
    data_.push_back(value);
    return *this;
}

Matrix2D &
Matrix2D::operator,(double value) {
    // The ordering is such that the upper left corner is the origin.
    common_NS::reporting::checkUppderBound(data_.size(), rows_ * cols_ - 1);
    data_.push_back(value);
    return *this;
}

void
Matrix2D::swap(Matrix2D const & in) {
    // to enable the safe exception guarantee
    rows_ = in.rows_;
    cols_ = in.cols_;
    data_ = in.data_;
}

Matrix2D::size_type
Matrix2D::rows() const {
    return rows_;
}

Matrix2D::size_type
Matrix2D::cols() const {
    return cols_;
}

namespace {
    Matrix2D::size_type row_major_offset(Matrix2D::size_type row, Matrix2D::size_type col, Matrix2D::size_type row_size) {
        return row * row_size + col;
    }

}

double
Matrix2D::operator()(Matrix2D::size_type row, Matrix2D::size_type col) const {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row, rows() - 1, "Matrix2D::operator(): Out of range error");
    common_NS::reporting::checkUppderBound(col, cols() - 1, "Matrix2D::operator(): Out of range error");
#endif
    size_type index = row * cols_ + col;
    return data_[index];
}

double &
Matrix2D::operator()(Matrix2D::size_type row, Matrix2D::size_type col) {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row, rows() - 1, "Matrix2D::operator(): Out of range error");
    common_NS::reporting::checkUppderBound(col, cols() - 1, "Matrix2D::operator(): Out of range error");
#endif
    Matrix2D::size_type index = row * cols_ + col;
    return data_[index];
}

void
Matrix2D::solve(Vector const & b, Vector & x) const {
    // solve A b, return in x
    common_NS::reporting::checkConditional(b.size() == cols_ && b.size() == x.size());
    for (size_type row = 0; row < rows_; ++row) {
        double tmp = 0;
        for (size_type col = 0; col < cols_; ++col)
            tmp += (*this)(row, col) * b(col);
        x(row) = tmp;
    }
}

void
Matrix2D::print() const {
    std::cout << std::endl;
    for (IMatrix2D::size_type i = 0; i < rows_; ++i) {
        for (IMatrix2D::size_type j = 0; j < cols_; ++j) {
//            unsigned int index = i * cols_ + j;
            std::cout << std::setw(15) << (*this)(i, j);
        }
        std::cout << std::endl;
    }
}

Matrix2D
Matrix2D::identity(Matrix2D::size_type n) {
    Matrix2D ident(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            ident(i, j) = i == j ? 1.0 : 0.0;
    }
    return ident;
}

} // namespace LinAlg_NS
