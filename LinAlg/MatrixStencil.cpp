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
LinAlg_NS::MatrixStencil::MapTo2D(unsigned short index) {
    common_NS::reporting::checkUppderBound(static_cast<decltype(values_.size())>(index), values_.size());
    common_NS::reporting::checkLowerBound(index, static_cast<decltype(index)>(index));

    short dim = static_cast<short>(std::sqrt(values_.size()));
    short offset = - dim / 2;
    short x = offset + index % dim;
    short y = offset + index / dim;
    return maptype_t(x, y);
}
