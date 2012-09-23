#include "pch.h"

#include "MatrixDecomposition.h"

using namespace LinearSolverLibrary_NS;


MatrixDecomposition::MatrixDecomposition(LinAlg_NS::SparseMatrix2D const & m, DataType const & is_data)
    :
    m_(m),
    is_data_(is_data) {}

MatrixDecomposition::const_iterator
MatrixDecomposition::cbegin() const {
    return is_data_.cbegin();
}

MatrixDecomposition::const_iterator
MatrixDecomposition::cend() const {
    return is_data_.cend();
}

LinAlg_NS::SparseMatrix2D const &
MatrixDecomposition::matrix() const {
    return m_;
}
