#include "pch.h"

#include "AMGStandardCoarseningTest.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.h"


using namespace LinAlg_NS;


void
AMGStandardCoarseningTest::setUp() {}

void
AMGStandardCoarseningTest::tearDown() {}

void
AMGStandardCoarseningTest::Test1() {
    MatrixStencil stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    Matrix2D reference_matrix(9, 9);
    reference_matrix <<
        4, -1, 0, -1, 0, 0, 0, 0, 0,
        -1, 4, -1, 0, -1, 0, 0, 0, 0,
        0, -1, 4, 0, 0, -1, 0, 0, 0,
        -1, 0, 0, 4, -1, 0, -1, 0, 0,
        0, -1, 0, -1, 4, -1, 0, -1, 0,
        0, 0, -1, 0, -1, 4, 0, 0, -1,
        0, 0, 0, -1, 0, 0, 4, -1, 0,
        0, 0, 0, 0, -1, 0, -1, 4, -1,
        0, 0, 0, 0, 0, -1, 0, -1, 4;



}
