/*
* Name  : PeriodicBCMatrixStencil
* Path  :
* Use   : Constructs a sparse matrix from a stencil with periodic b.c.
          For each grid point (in the geometric grid, NOT the matrix),
          we place the stencil and insert the stencil values in the
          current row corresponding to the grid point in the matrix.
          The ordering is such that the upper left corner is the origin.
          The imposed boundary conditions are of type periodic, i.e. the
          last right column maps to the 1st one and the bottom grid row
          maps to the 1st row.
          The stencil only applies to inner (non-boundary) grid nodes.
* Author: Sven Schmidt
* Date  : 11/10/2013
*/
#pragma once


#include "LinAlg/IMatrix2D.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/DeclSpec.h"

#include <vector>
#include <tuple>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {


class LINALG_DECL_SYMBOLS PeriodicBCMatrixStencil
{
    friend class MatrixStencilTest;


public:
    typedef std::tuple<short, short> maptype_t;

public:
    PeriodicBCMatrixStencil();

    PeriodicBCMatrixStencil & operator<<(double value);
    PeriodicBCMatrixStencil & operator,(double value);

    LinAlg_NS::SparseMatrix2D generateMatrix(unsigned short matrixDimension) const;

private:
    maptype_t                 mapTo2D(unsigned short index) const;
    unsigned short            mapToIndex(short i, short j) const;
    void                      applyStencil(IMatrix2D::size_type currentRow, LinAlg_NS::SparseMatrix2D & m) const;
    void                      generateGridNodeMapping();

private:
    std::vector<double> values_;
};


} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
