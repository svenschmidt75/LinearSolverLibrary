/*
* Name  : PeriodicBoundaryConditionPolicy
* Path  :
* Use   : Policy for periodic b.c. for generating
*         matrices from matrix stencils.
*         The 1st column is mapped to the last column, which is only virtual.
*         The 1st row is mapped to the last row, which is also virtual.
*         The virtual column and row are NOT part of the matrix, i.e.
*         a 3x3 matrix with periodic b.c. is virtually extended to 4x4.
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


    struct PeriodicBoundaryConditionPolicy {
    public:
        typedef std::tuple<IMatrix2D::size_type, IMatrix2D::size_type> MapType_t;

    public:
        bool
        isMatrixPositionValid(IMatrix2D::size_type matrixRow, IMatrix2D::size_type matrixColumn, short stencilX, short stencilY) const {
//             if (matrixColumn + stencilX >= row_size_ + 1)
//                 return false;
//             if (matrixRow + stencilY >= row_size_ + 1)
//                 return false;
            return true;
        }

        MapType_t
        getMappedColumnIndex(IMatrix2D::size_type matrixRow, IMatrix2D::size_type matrixColumn, short stencilX, short stencilY) const {
            common_NS::reporting::checkConditional(isMatrixPositionValid(matrixRow, matrixColumn, stencilX, stencilY));
            auto column = matrixColumn + stencilX;
            if (column == row_size_)
                column = 0;
            auto row = matrixRow + stencilY;
            if (row == row_size_)
                row = 0;
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
