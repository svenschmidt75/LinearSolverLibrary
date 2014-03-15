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
    AMGVCycle cycle;
    AMGMonitor monitor;
    monitor.direct_solver_threshold_ = 3;

    AMGSolver<AMGDirectInterpolationPolicy> amg_solver{m_, b, cycle, monitor};

    ASSERT_THAT(amg_solver.amg_levels_.size(), Eq(3));
}

TEST_F(BasicAMGSolverTest, TestExpectedVCycleDepth) {
    Vector b;
    AMGVCycle cycle;
    AMGMonitor monitor;
    monitor.direct_solver_threshold_ = 3;

    AMGSolver<AMGDirectInterpolationPolicy> amg_solver{ m_, b, cycle, monitor };

    // test all the cycle indices!
    std::vector<int> cycle_levels;
    cycle_levels.assign(std::cbegin(cycle), std::cend(cycle));

    ASSERT_THAT(cycle_levels, ElementsAre(0, 1, 2, 3, 2, 1, 0));
}
