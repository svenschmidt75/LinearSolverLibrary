#include "pch.h"

#include "PeriodicBCMatrixStencil.h"

#include "common/reporting.h"


using namespace LinAlg_NS;


PeriodicBCMatrixStencil::PeriodicBCMatrixStencil() {}

PeriodicBCMatrixStencil &
PeriodicBCMatrixStencil::operator<<(double value) {
    values_.push_back(value);
    return *this;
}

PeriodicBCMatrixStencil &
PeriodicBCMatrixStencil::operator,(double value) {
    values_.push_back(value);
    return *this;
}

PeriodicBCMatrixStencil::maptype_t
LinAlg_NS::PeriodicBCMatrixStencil::mapTo2D(unsigned short index) const {
    common_NS::reporting::checkUppderBound(static_cast<decltype(values_.size())>(index), values_.size());
    common_NS::reporting::checkLowerBound(index, static_cast<decltype(index)>(index));

    short dim = static_cast<short>(std::sqrt(values_.size()));
    short offset = -dim / 2;
    short x = offset + index % dim;
    short y = offset + index / dim;
    return std::make_tuple(x, y);
}

unsigned short
PeriodicBCMatrixStencil::mapToIndex(short i, short j) const {
    short dim = static_cast<short>(std::sqrt(values_.size()));
    short offset = -dim / 2;
    common_NS::reporting::checkUppderBound(static_cast<int>(i), -offset);
    common_NS::reporting::checkLowerBound(i, offset);
    common_NS::reporting::checkUppderBound(static_cast<int>(j), -offset);
    common_NS::reporting::checkLowerBound(j, offset);
    short index = (-offset + j) * dim + (-offset + i);
    return index;
}

void
PeriodicBCMatrixStencil::generateGridNodeMapping() {

}

LinAlg_NS::SparseMatrix2D
PeriodicBCMatrixStencil::generateMatrix(unsigned short matrixDimension) const {
    short dim = static_cast<short>(std::sqrt(values_.size()));
//    common_NS::reporting::checkLowerBound(matrixDimension, dim);
    common_NS::reporting::checkConditional(values_.size() % 2);
    common_NS::reporting::checkConditional(dim * dim == values_.size());
    SparseMatrix2D m(matrixDimension);

//    generateGridNodeMapping();


    for (IMatrix2D::size_type currentRow = 0; currentRow < matrixDimension; ++currentRow)
        applyStencil(currentRow, m);
    m.finalize();
    m.print();
    return m;
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

void
PeriodicBCMatrixStencil::applyStencil(IMatrix2D::size_type currentRow, SparseMatrix2D & m) const {
    // Insert the stencil values for the grid point into the matrix.
    // Note that the grid point is represented in the matrix via the
    // row currentRow, i.e. only that row is modified in the matrix.
    short row_size = static_cast<short>(std::sqrt(m.rows()));
    short row;
    short col;
    std::tie(row, col) = mapIndexTo2D(static_cast<short>(currentRow), row_size);
    for (size_t index = 0; index < values_.size(); ++index) {
        short x;
        short y;
        std::tie(x, y) = mapTo2D(static_cast<short>(index));
        if (isStencilLocationWithinGridBounds(col, row, x, y, row_size) == false)
            continue;
        IMatrix2D::size_type x_offset = currentRow + x;
        IMatrix2D::size_type y_offset = x_offset + y * row_size;
        m(currentRow, y_offset) = values_[index];
    }
}
