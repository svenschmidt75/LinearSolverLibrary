#include "pch.h"


#include "SparseMatrixSymmetryDecorator.h"


using namespace LinAlg_NS;


namespace EntityReader_NS {

SparseMatrixSymmetryDecorator::SparseMatrixSymmetryDecorator(LinAlg_NS::SparseMatrix2D & in)
    :
    m_(in) {}

SparseMatrix2D::size_type
SparseMatrixSymmetryDecorator::rows() const {
    return m_.rows();
}

SparseMatrix2D::size_type
SparseMatrixSymmetryDecorator::cols() const {
    return m_.cols();
}

double
SparseMatrixSymmetryDecorator::operator()(size_type row, size_type col) const {
    return m_(row, col);
}

double &
SparseMatrixSymmetryDecorator::operator()(size_type row, size_type col) {
    return m_(row, col);

}

void
SparseMatrixSymmetryDecorator::solve(LinAlg_NS::Vector const & b, LinAlg_NS::Vector & x) {

}

void
SparseMatrixSymmetryDecorator::print() const {

}

} // EntityReader_NS
