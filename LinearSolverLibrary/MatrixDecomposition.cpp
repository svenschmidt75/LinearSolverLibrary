#include "pch.h"

#include "MatrixDecomposition.h"


using namespace LinearSolverLibrary_NS;


MatrixDecomposition::MatrixDecomposition() {}

MatrixDecomposition::MatrixDecomposition(ISO_t const & iso_data)
    :
    iso_data_(iso_data) {}

MatrixDecomposition::MatrixDecomposition(MatrixDecomposition && in)
    :
    iso_data_(std::move(in.iso_data_)) {}

MatrixDecomposition &
MatrixDecomposition::operator=(MatrixDecomposition && in) {
    iso_data_ = std::move(in.iso_data_);
    return *this;
}

MatrixDecomposition::const_iterator
MatrixDecomposition::begin() const {
    return std::cbegin(iso_data_);
}

MatrixDecomposition::const_iterator
MatrixDecomposition::end() const {
    return std::cend(iso_data_);
}

MatrixDecomposition::size_type
MatrixDecomposition::size() const {
    return iso_data_.size();
}
