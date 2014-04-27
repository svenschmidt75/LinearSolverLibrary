#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGSolver.hpp"


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
        success = splitting_policy.Generate(m_);
    }

public:
    SparseMatrix2D               m_;
    AMGDirectInterpolationPolicy splitting_policy;
    bool                         success;
};


TEST_F(BasicAMGSolverTest, TestExpectedGridHierarchyDepth) {
    Vector b;
    AMGMonitor monitor;
    monitor.direct_solver_threshold = 3;

    AMGSolver<AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

    ASSERT_THAT(amg_solver.amg_levels_.size(), Eq(3));
}

TEST_F(BasicAMGSolverTest, TestExpectedVCycleDepth) {
    Vector b;
    AMGMonitor monitor;
    monitor.direct_solver_threshold = 3;

    AMGSolver<AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

    // test all the cycle indices!
    std::vector<int> cycle_levels;
    cycle_levels.assign(std::cbegin(amg_solver.cycle_scheme_), std::cend(amg_solver.cycle_scheme_));

    ASSERT_THAT(cycle_levels, ElementsAre(0, 1, 2, 1, 0));
}

TEST_F(BasicAMGSolverTest, TestVSolve) {
    AMGMonitor monitor;
    monitor.direct_solver_threshold = 3;
    monitor.nmax_iterations = 101;
    monitor.nu1 = monitor.nu2 = 1;

    double tolerance = 1E-16;
    monitor.required_tolerance = tolerance;


    Vector b{m_.cols()};
    std::fill(std::begin(b), std::end(b), 1);

    AMGSolver<AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

    m_.print();

    Vector x{b.size()};
    std::fill(std::begin(x), std::end(x), 0);
    bool success;
    std::tie(success, x) = amg_solver.Solve(x);
    ASSERT_TRUE(success);

    Vector x_ref{b.size()};
    int iterations;
    std::tie(success, x_ref, iterations) = SparseLinearSolverLibrary::SparseSOR(m_, b, 1.1, 11000);
    ASSERT_TRUE(success);

    // compare vectors
    ASSERT_TRUE(SparseLinearSolverUtil::isVectorEqual(x, x_ref, tolerance));
}
