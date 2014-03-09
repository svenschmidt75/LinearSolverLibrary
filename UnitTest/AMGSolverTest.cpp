#include "pch.h"

#include "LinearSolverLibrary/AMGSolver.hpp"
#include "LinearSolverLibrary/AMGVCycle.h"
#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(AMGSolverTest, Test1) {
    SparseMatrix2D m;
    Vector b;
    AMGVCycle cycle;
    AMGMonitor monitor;


    AMGSolver<AMGDirectInterpolationPolicy> amg_solver{m, b, cycle, monitor};

}
