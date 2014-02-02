#include "pch.h"

#include "AMGPolicyTest.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGPolicy.hpp"
#include "LinearSolverLibrary/AMGStandardSplittingPolicy.h"
#include "LinearSolverLibrary/AMGStandardSplitting.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGPolicyTest::setUp() {}

void
AMGPolicyTest::tearDown() {}

void
AMGPolicyTest::test1() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    AMGStandardSplittingPolicy splitting_policy;
    splitting_policy.generate(m);
}
