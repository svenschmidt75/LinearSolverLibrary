#include "pch.h"

#include "MatrixStencil.h"

#include "common/reporting.h"


using namespace LinAlg_NS;


MatrixStencil::MatrixStencil() {
}

MatrixStencil &
MatrixStencil::operator<<(double value) {
    values_.push_back(value);
    return *this;
}

MatrixStencil &
MatrixStencil::operator,(double value) {
    values_.push_back(value);
    return *this;
}

MatrixStencil::maptype_t
LinAlg_NS::MatrixStencil::MapTo2D(unsigned short index) const {
    common_NS::reporting::checkUppderBound(static_cast<decltype(values_.size())>(index), values_.size());
    common_NS::reporting::checkLowerBound(index, static_cast<decltype(index)>(index));

    short dim = static_cast<short>(std::sqrt(values_.size()));
    short offset = - dim / 2;
    short x = offset + index % dim;
    short y = offset + index / dim;
    return maptype_t(x, y);
}

LinAlg_NS::SparseMatrix2D
LinAlg_NS::MatrixStencil::generateMatrix(short matrixDimension) const {
    short dim = static_cast<short>(std::sqrt(values_.size()));
//    common_NS::reporting::checkLowerBound(matrixDimension, dim);
    common_NS::reporting::checkConditional(values_.size() % 2);
    common_NS::reporting::checkConditional(dim * dim == values_.size());

    SparseMatrix2D m(matrixDimension);

    for (IMatrix2D::size_type i = 0; i < matrixDimension; ++i) {
        
    }






    m.finalize();
    return m;
}

unsigned short
LinAlg_NS::MatrixStencil::MapToIndex(short i, short j) const {
}
