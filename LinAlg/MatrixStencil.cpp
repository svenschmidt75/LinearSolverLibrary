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
LinAlg_NS::MatrixStencil::mapTo2D(unsigned short index) const {
    common_NS::reporting::checkUppderBound(static_cast<decltype(values_.size())>(index), values_.size());
    common_NS::reporting::checkLowerBound(index, static_cast<decltype(index)>(index));

    short dim = static_cast<short>(std::sqrt(values_.size()));
    short offset = - dim / 2;
    short x = offset + index % dim;
    short y = offset + index / dim;
    return maptype_t(x, y);
}

unsigned short
LinAlg_NS::MatrixStencil::mapToIndex(short i, short j) const {
    short dim = static_cast<short>(std::sqrt(values_.size()));
    short offset = -dim / 2;
    common_NS::reporting::checkUppderBound(static_cast<int>(i), -offset);
    common_NS::reporting::checkLowerBound(i, offset);
    common_NS::reporting::checkUppderBound(static_cast<int>(j), -offset);
    common_NS::reporting::checkLowerBound(j, offset);
    short index = (-offset + j) * dim + (-offset + i);
    return index;
}

namespace {

    std::tuple<short, short> mapIndexTo2D(unsigned short index, unsigned short dimension) {
        short row = index / dimension;
        short column = index % dimension;
        return std::make_tuple(row, column);
    }


    bool isStencilLocationWithinGridBounds(short currentMatrixColumn, short currentMatrixRow, short stencilX, short stencilY, short matrixRowSize) {
        if (currentMatrixColumn + stencilX < 0 || currentMatrixColumn + stencilX >= matrixRowSize)
            return false;
        if (currentMatrixRow + stencilY < 0 || currentMatrixRow + stencilY >= matrixRowSize)
            return false;
        return true;
    }

}

LinAlg_NS::SparseMatrix2D
LinAlg_NS::MatrixStencil::generateMatrix(unsigned short matrixDimension) const {
    short dim = static_cast<short>(std::sqrt(values_.size()));
//    common_NS::reporting::checkLowerBound(matrixDimension, dim);
    common_NS::reporting::checkConditional(values_.size() % 2);
    common_NS::reporting::checkConditional(dim * dim == values_.size());

    SparseMatrix2D m(matrixDimension);
    short row_size = static_cast<short>(std::sqrt(matrixDimension));

    for (IMatrix2D::size_type i = 0; i < matrixDimension; ++i) {
        short row;
        short col;
        std::tie(row, col) = mapIndexTo2D(static_cast<short>(i), row_size);

        for (size_t index = 0; index < values_.size(); ++index) {
            short x;
            short y;
            std::tie(x, y) = mapTo2D(static_cast<short>(index));
            if (isStencilLocationWithinGridBounds(col, row, x, y, row_size) == false)
                continue;
            IMatrix2D::size_type x_offset = i + x;
            IMatrix2D::size_type y_offset = i + y;
            y_offset = x_offset + y * row_size;
            m(i, y_offset) = values_[index];
        }
    }
    m.finalize();
    m.print();
    return m;
}
