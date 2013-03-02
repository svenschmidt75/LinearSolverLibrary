#include "pch.h"

#include "helper.h"

#include "Vector.h"
#include "TransposeVector.h"
#include "Matrix2D.h"
#include "SparseMatrix2D.h"


namespace LinAlg_NS {

TransposeVector
helper::transpose(Vector const & vec) {
    TransposeVector tv(vec.size());
    tv.vector_.data_ = vec.data_;
    return tv;
}

Vector
helper::transpose(TransposeVector const & vec) {
    Vector tv(vec.vector_);
    return tv;

}

double
helper::get_value(Matrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col) {
    double value = m(row, col);
    return value;
}

double
helper::get_value(SparseMatrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col) {
    double value = m(row, col);
    return value;
}

bool
helper::isSymmsteric(SparseMatrix2D const & m) {
    static double const tol = 1E-10;

    for (IMatrix2D::size_type row = 1; row < m.cols(); ++row) {
        for (IMatrix2D::size_type col = 0; col < row; ++col) {
            double a_ij = m(row, col);
            double a_ji = m(col, row);
            double delta = std::fabs(a_ij - a_ji);

            if (delta > tol)
                return false;
        }
    }

    return true;
}

} // namespace LinAlg_NS
