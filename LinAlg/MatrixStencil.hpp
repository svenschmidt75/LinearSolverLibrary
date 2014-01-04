/*
* Name  : MatrixStencil
* Path  :
* Use   : Constructs a sparse matrix from a stencil with Dirichlet b.c.
          For each grid point (in the geometric grid, NOT the matrix),
          we place the stencil and insert the stencil values in the
          current row corresponding to the grid point in the matrix.
          The ordering is such that the upper left corner is the origin.
          All b.c. are assumed to be Dirichlet-type. The stencil then only
          applies to inner (non-boundary) grid nodes.
* Author: Sven Schmidt
* Date  : 10/05/2013
*/
#pragma once


#include "LinAlg/IMatrix2D.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/DeclSpec.h"


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {

template<typename BOUNDARY_CONDITION_POLICY>
class MatrixStencil {

    friend class MatrixStencilTest;


public:
    typedef std::tuple<short, short> MapType_t;

public:
    MatrixStencil() : bc_policy_{} {}

    MatrixStencil & operator<<(double value) {
        values_.push_back(value);
        return *this;
    }

    MatrixStencil & operator,(double value) {
        values_.push_back(value);
        return *this;
    }

    LinAlg_NS::SparseMatrix2D generateMatrix(unsigned short matrix_size) const {
        unsigned short stencil_dimension = static_cast<short>(std::sqrt(values_.size()));
        unsigned short matrix_dimension = static_cast<short>(std::sqrt(matrix_size));
        common_NS::reporting::checkLowerBound(matrix_size, stencil_dimension);
        common_NS::reporting::checkConditional(values_.size() % 2);
        common_NS::reporting::checkConditional(stencil_dimension * stencil_dimension == values_.size());
        bc_policy_.setRowSize(matrix_dimension);
        SparseMatrix2D m{matrix_size};
        SparseMatrix2D check_matrix{matrix_size};
        for (IMatrix2D::size_type currentRow{0}; currentRow < matrix_size; ++currentRow)
            applyStencil(currentRow, m);
        m.finalize();
//        m.print();
        return m;
    }

private:
    unsigned short
    mapStencilCoordinatesToIndex(short i, short j) const {
        short dim = static_cast<short>(std::sqrt(values_.size()));
        short offset = -dim / 2;
        common_NS::reporting::checkUppderBound(static_cast<int>(i), -offset);
        common_NS::reporting::checkLowerBound(i, offset);
        common_NS::reporting::checkUppderBound(static_cast<int>(j), -offset);
        common_NS::reporting::checkLowerBound(j, offset);
        short index = (-offset + j) * dim + (-offset + i);
        return index;
    }

    MapType_t
    mapIndexToStencilCoordinates(unsigned short index) const {
        // Maps flat index to 2D stencil coordinates with origin
        // at at the center. For example, if the stencil is 3x3,
        // the center is at (1,1).
        common_NS::reporting::checkUppderBound(static_cast<decltype(values_.size())>(index), values_.size());
        common_NS::reporting::checkLowerBound(index, static_cast<decltype(index)>(index));
        short dim = static_cast<short>(std::sqrt(values_.size()));
        short offset = -dim / 2;
        short x = offset + index % dim;
        short y = offset + index / dim;
        return std::make_tuple(x, y);
    }

    std::tuple<IMatrix2D::size_type, IMatrix2D::size_type>
    mapIndexToMatrixPosition(IMatrix2D::size_type index, IMatrix2D::size_type row_size) const {
        auto x = index / row_size;
        auto y = index % row_size;
        return std::make_tuple(x, y);
    }

    void applyStencil(IMatrix2D::size_type matrix_row, LinAlg_NS::SparseMatrix2D & m) const {
        // Insert the stencil values for the grid point into the matrix.
        // Note that the grid point is represented in the matrix via the
        // row currentRow, i.e. only that row is modified in the matrix.
        auto row_size = static_cast<IMatrix2D::size_type>(std::sqrt(m.rows()));
        IMatrix2D::size_type baseMatrixRow;
        IMatrix2D::size_type baseMatrixColumn;

        auto & writable_check_matrix = const_cast<SparseMatrix2D &>(check_matrix_);

        // center stencil around this matrix position
        std::tie(baseMatrixRow, baseMatrixColumn) = mapIndexToMatrixPosition(matrix_row, row_size);
        for (short stencil_index = 0; stencil_index < static_cast<short>(values_.size()); ++stencil_index) {
            short stencilX;
            short stencilY;
            std::tie(stencilX, stencilY) = mapIndexToStencilCoordinates(stencil_index);
            if (!bc_policy_.isMatrixPositionValid(baseMatrixRow, baseMatrixColumn, stencilX, stencilY))
                continue;
            IMatrix2D::size_type matrixRow;
            IMatrix2D::size_type matrixColumn;
            std::tie(matrixRow, matrixColumn) = bc_policy_.getMappedColumnIndex(baseMatrixRow, baseMatrixColumn, stencilX, stencilY);
            auto mapped_column_index = matrixColumn + matrixRow * row_size;
            if (check_matrix_(matrix_row, mapped_column_index)) {
                // Matrix element m(matrix_row, mapped_column_index) already set.
                // Either then stencil is too large, or the matrix is too small.
                // This happens if the stencil is larger than the matrix, i.e.
                // the stencil is 5x5, but the matrix is only 4x4.
                boost::format format = boost::format("\nMatrixStencil::applyStencil: Matrix element (%1%, %2%)=%3% already set while applying stencil at (%4%, %5%)=%6%") % matrix_row % mapped_column_index % m(matrix_row, mapped_column_index) % stencilX % stencilY % values_[stencil_index];
                common_NS::reporting::error(format.str());
                throw std::runtime_error(format.str());
            }
            if (values_[stencil_index])
                m(matrix_row, mapped_column_index) = values_[stencil_index];
            writable_check_matrix(matrix_row, mapped_column_index) = 1.0;
        }
    }

private:
    std::vector<double>               values_;
    SparseMatrix2D                    check_matrix_;
    mutable BOUNDARY_CONDITION_POLICY bc_policy_;
};


} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
