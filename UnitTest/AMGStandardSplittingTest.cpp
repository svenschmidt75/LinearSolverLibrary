#include "pch.h"

#include "AMGStandardSplittingTest.h"

#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinearSolverLibrary/VariableCardinalityPolicy.h"
#include "LinearSolverLibrary/VariableInfluenceAccessor.h"
#include "LinearSolverLibrary/AMGStandardCoarseningStrengthPolicy.h"
#include "LinearSolverLibrary/AMGStandardSplitting.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.h"

using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGStandardSplittingTest::setUp() {}

void
AMGStandardSplittingTest::tearDown() {}

void
AMGStandardSplittingTest::Test1() {
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
    AMGStandardSplitting splitting(m, influence_accessor, variable_categorizer);
    splitting.generateSplitting();

}
