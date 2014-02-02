#include "pch.h"

#include "AMGDirectInterpolationPolicyTest.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"
#include "LinearSolverLibrary/AMGStandardSplitting.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGDirectInterpolationPolicyTest::setUp() {}

void
AMGDirectInterpolationPolicyTest::tearDown() {}

void
AMGDirectInterpolationPolicyTest::test1() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    AMGDirectInterpolationPolicy splitting_policy;
    splitting_policy.generate(m);
}
