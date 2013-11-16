/*
* Name  : DirichletBoundaryConditionPolicy
* Path  :
* Use   : Policy for Dirichlet b.c. for generating
*         matrices from matrix stencils.
* Author: Sven Schmidt
* Date  : 11/16/2013
*/
#pragma once


#include "LinAlg/IMatrix2D.h"

#include "common/reporting.h"

#include <tuple>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {


struct DirichletBoundaryConditionPolicy {
public:
    typedef std::tuple<IMatrix2D::size_type, IMatrix2D::size_type> MapType_t;

public:
    DirichletBoundaryConditionPolicy() {}

    bool
    isMatrixPositionValid(IMatrix2D::size_type matrixRow, IMatrix2D::size_type matrixColumn, short stencilX, short stencilY) const {
        if (matrixColumn + stencilX >= row_size_)
            return false;
        if (matrixRow + stencilY >= row_size_)
            return false;
        return true;
    }

    MapType_t
    getMappedColumnIndex(IMatrix2D::size_type matrixRow, IMatrix2D::size_type matrixColumn, short stencilX, short stencilY) const {
        common_NS::reporting::checkConditional(isMatrixPositionValid(matrixRow, matrixColumn, stencilX, stencilY));
        auto column = matrixColumn + stencilX;
        auto row = matrixRow + stencilY;
        return std::make_tuple(row, column);
    }

    void setRowSize(size_t row_size) {
        row_size_ = row_size;
    }

private:
    size_t row_size_;
};


} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
