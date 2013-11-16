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
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/DeclSpec.h"

#include "common/reporting.h"

#include <vector>
#include <tuple>

#include <boost/optional.hpp>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {


class DirichletBoundaryConditionPolicy {
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

    boost::optional<MapType_t>
    mapStencilToMatrixPosition(short stencilX, short stencilY, short matrixX, short matrixY) {
        short row_size = static_cast<short>(std::sqrt(row_size_));
        boost::optional<MapType_t> matrixPosition{};
        if (isStencilLocationWithinGridBounds(stencilX, stencilY, matrixX, matrixY, row_size) == false)
            return matrixPosition;
        IMatrix2D::size_type x_offset = matrixY * row_size + matrixX + stencilX;
        IMatrix2D::size_type y_offset = x_offset + stencilY * row_size;
        auto coordinates = std::make_tuple(x_offset, y_offset);
        return boost::optional<MapType_t>{coordinates};
    }

    void setRowSize(size_t row_size) {
        row_size_ = row_size;
    }

private:
    bool
    isStencilLocationWithinGridBounds(short stencilX, short stencilY, short matrixX, short matrixY, short matrixRowSize) {
        if (matrixX + stencilX < 0 || matrixX + stencilX >= matrixRowSize)
            return false;
        if (matrixY + stencilY < 0 || matrixY + stencilY >= matrixRowSize)
            return false;
        return true;
    }

private:
    size_t row_size_;
};


} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
