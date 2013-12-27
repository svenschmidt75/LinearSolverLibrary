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


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {


    struct PeriodicBoundaryConditionPolicy {
    public:
        typedef std::tuple<IMatrix2D::size_type, IMatrix2D::size_type> MapType_t;

    public:
        bool
        isMatrixPositionValid(IMatrix2D::size_type matrixRow, IMatrix2D::size_type matrixColumn, short stencilX, short stencilY) const {
            // always valid for periodic b.c.
            return true;
        }

        MapType_t
        getMappedColumnIndex(IMatrix2D::size_type matrixRow, IMatrix2D::size_type matrixColumn, short stencilX, short stencilY) const {
            stencilX = WrapStencilPosition(stencilX);
            auto column = ComputeMatrixCoordinate(matrixColumn, stencilX);
            stencilY = WrapStencilPosition(stencilY);
            auto row = ComputeMatrixCoordinate(matrixRow, stencilY);
            common_NS::reporting::checkUppderBound(row, row_size_);
            common_NS::reporting::checkUppderBound(column, row_size_);
            return std::make_tuple(row, column);
        }

        void setRowSize(size_t row_size) {
            row_size_ = row_size;
        }

    private:
        IMatrix2D::size_type
        ComputeMatrixCoordinate(IMatrix2D::size_type matrix_coordinate, short stencil_offset) const {
            auto mapped_matrix_coordinate = matrix_coordinate + stencil_offset;
            if (mapped_matrix_coordinate > matrix_coordinate + std::abs(stencil_offset)) {
                // underflow
                mapped_matrix_coordinate = row_size_ - std::abs(stencil_offset);
            }
            mapped_matrix_coordinate %= row_size_;
            return mapped_matrix_coordinate;
        }

        short
        WrapStencilPosition(short stencil_coordinate) const {
            if (stencil_coordinate < 0) {
                short m = static_cast<short>(-stencil_coordinate / row_size_);
                if (-stencil_coordinate % row_size_)
                    m++;
                stencil_coordinate += static_cast<short>(m * row_size_);
            }
            else
                stencil_coordinate %= row_size_;
            common_NS::reporting::checkLowerBound(stencil_coordinate, static_cast<short>(0));
            common_NS::reporting::checkUppderBound(stencil_coordinate, static_cast<short>(row_size_ - 1));
            return stencil_coordinate;
        }

    private:
        size_t row_size_;
    };


} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
