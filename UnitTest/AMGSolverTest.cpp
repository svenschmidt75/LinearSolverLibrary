#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGSolver.hpp"
#include "LinearSolverLibrary/AMGVCycle.h"
#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


class BasicAMGSolverTest : public Test {
public:
    void SetUp() override {
        MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
        stencil << 0, -1,  0,
                  -1,  4, -1,
                   0, -1,  0;
        m_ = stencil.generateMatrix(3 * 3);
        success = splitting_policy.generate(m_);
    }

public:
    SparseMatrix2D               m_;
    AMGDirectInterpolationPolicy splitting_policy;
    bool                         success;
};


TEST_F(BasicAMGSolverTest, TestExpectedGridHierarchyDepth) {
    Vector b;
    AMGMonitor monitor;
    monitor.direct_solver_threshold_ = 3;

    AMGSolver<AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

    ASSERT_THAT(amg_solver.amg_levels_.size(), Eq(3));
}

TEST_F(BasicAMGSolverTest, TestExpectedVCycleDepth) {
    Vector b;
    AMGMonitor monitor;
    monitor.direct_solver_threshold_ = 3;

    AMGSolver<AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

    // test all the cycle indices!
    std::vector<int> cycle_levels;
    cycle_levels.assign(std::cbegin(amg_solver.cycle_scheme_), std::cend(amg_solver.cycle_scheme_));

    ASSERT_THAT(cycle_levels, ElementsAre(0, 1, 2, 3, 2, 1, 0));
}

TEST_F(BasicAMGSolverTest, TestVSolve) {
    Vector b{3};
    b(0) = 1;
    b(1) = 1;
    b(2) = 1;

    AMGMonitor monitor;
    monitor.direct_solver_threshold_ = 3;

    AMGSolver<AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

    Vector x{3};
    x(0) = 0;
    x(1) = 0;
    x(2) = 0;
//    x = amg_solver.solve(x);

}
