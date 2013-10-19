#include "pch.h"

#include "AMGVariableCardinalityTest.h"

#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinearSolverLibrary/VariableCardinalityPolicy.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGVariableCardinalityTest::setUp() {}

void
AMGVariableCardinalityTest::tearDown() {}

void
AMGVariableCardinalityTest::Test1() {
    MatrixStencil stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();
/*
      4       -1        0       -1        0        0        0        0        0
     -1        4       -1        0       -1        0        0        0        0
      0       -1        4        0        0       -1        0        0        0
     -1        0        0        4       -1        0       -1        0        0
      0       -1        0       -1        4       -1        0       -1        0
      0        0       -1        0       -1        4        0        0       -1
      0        0        0       -1        0        0        4       -1        0
      0        0        0        0       -1        0       -1        4       -1
      0        0        0        0        0       -1        0       -1        4
*/
    VariableCategorizer variableCategorizer(m.rows());
    VariableCardinalityPolicy cardinalityPolicy(variableCategorizer);
}
