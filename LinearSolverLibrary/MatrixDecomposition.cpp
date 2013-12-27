#include "pch.h"

#include "MatrixDecomposition.h"


using namespace LinearSolverLibrary_NS;


MatrixDecomposition::MatrixDecomposition(ISO_t const & iso_data)
    :
    iso_data_(iso_data) {}

MatrixDecomposition::MatrixDecomposition(MatrixDecomposition && in)
    :
    iso_data_(std::move(in.iso_data_)) {}

MatrixDecomposition::const_iterator
MatrixDecomposition::begin() const {
    return iso_data_.cbegin();
}

MatrixDecomposition::const_iterator
MatrixDecomposition::end() const {
    return iso_data_.cend();
}

MatrixDecomposition::size_type
MatrixDecomposition::size() const {
    return iso_data_.size();
}
