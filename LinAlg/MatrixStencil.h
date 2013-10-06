/*
* Name  : MatrixStencil
* Path  :
* Use   : Constructs a sparse matrix from a stencil.
          For each grid point (in the geometric grid, NOT the matrix),
          we place the stencil and insert the stencil values in the
          current row corresponding to the grid point in the matrix.
          The ordering is such that the upper left corner is the origin.
* Author: Sven Schmidt
* Date  : 10/05/2013
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


class LINALG_DECL_SYMBOLS MatrixStencil
{
    friend class MatrixStencilTest;


public:
    typedef std::tuple<short, short> maptype_t;

public:
    MatrixStencil();

    MatrixStencil & operator<<(double value);
    MatrixStencil & operator,(double value);

private:
    maptype_t                 mapTo2D(unsigned short index) const;
    unsigned short            mapToIndex(short i, short j) const;
    LinAlg_NS::SparseMatrix2D generateMatrix(unsigned short matrixDimension) const;
    void                      applyStencil(IMatrix2D::size_type currentRow, LinAlg_NS::SparseMatrix2D & m) const;

private:
    std::vector<double> values_;
};


} // namespace LinAlg_NS


#pragma warning(default:4275)
#pragma warning(default:4251)
