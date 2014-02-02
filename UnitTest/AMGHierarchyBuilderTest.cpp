#include "pch.h"

#include "AMGHierarchyBuilderTest.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGHierarchyBuilder.hpp"
#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGHierarchyBuilderTest::setUp() {}

void
AMGHierarchyBuilderTest::tearDown() {}

void
AMGHierarchyBuilderTest::test1() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    AMGDirectInterpolationPolicy splitting_policy;
    AMGHierarchyBuilder<AMGDirectInterpolationPolicy> builder;

    auto amg_levels = builder.build(m);
}
