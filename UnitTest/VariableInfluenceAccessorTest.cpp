#include "pch.h"

#include "VariableInfluenceAccessorTest.h"
#include "LinearSolverLibrary/AMGStandardCoarseningStrengthPolicy.h"
#include "LinearSolverLibrary/VariableInfluenceAccessor.h"
#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
VariableInfluenceAccessorTest::setUp() {}

void
VariableInfluenceAccessorTest::tearDown() {}

void
VariableInfluenceAccessorTest::Test1() {
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

    AMGStandardCoarseningStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);

//    influence_accessor.GetVariableInfluencedFine()
//    influence_accessor.GetVariableInfluencedUndefined()





}
