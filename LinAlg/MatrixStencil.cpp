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

LinAlg_NS::SparseMatrix2D
LinAlg_NS::MatrixStencil::generateMatrix(short matrixDimension) const {
    short dim = static_cast<short>(std::sqrt(values_.size()));
//    common_NS::reporting::checkLowerBound(matrixDimension, dim);
    common_NS::reporting::checkConditional(values_.size() % 2);
    common_NS::reporting::checkConditional(dim * dim == values_.size());

    SparseMatrix2D m(matrixDimension);

    for (IMatrix2D::size_type i = 0; i < matrixDimension; ++i) {
        
        for (size_t index = 0; index < values_.size(); ++index) {
            short x;
            short y;
            std::tie(x, y) = mapTo2D(static_cast<short>(index));
            IMatrix2D::size_type x_offset = i + x;
            IMatrix2D::size_type y_offset = i + y;
            if (x_offset > matrixDimension || y_offset > matrixDimension)
                // underflow or beyond matrix size
                continue;
            m(x_offset, y_offset) = values_[index];
        }
    }

    m.finalize();
    return m;
}
