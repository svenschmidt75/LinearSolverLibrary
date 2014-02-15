#include "pch.h"

#include "AMGSolverTest.h"
#include "LinearSolverLibrary/AMGSolver.hpp"
#include "LinearSolverLibrary/AMGVCycle.h"
#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGSolverTest::setUp() {}

void
AMGSolverTest::tearDown() {}

void
AMGSolverTest::test1() {
    SparseMatrix2D m;
    Vector b;
    AMGVCycle cycle;
    AMGMonitor monitor;


    AMGSolver<AMGDirectInterpolationPolicy> amg_solver{m, b, cycle, monitor};

}
