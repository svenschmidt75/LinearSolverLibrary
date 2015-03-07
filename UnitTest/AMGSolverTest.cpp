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

        AMGStandardStrengthPolicy strength_policy{m_};
        VariableCategorizer variable_categorizer{m_.rows()};
        VariableInfluenceAccessor influence_accessor{strength_policy, variable_categorizer};
        AMGStandardCoarsening coarsening{m_, influence_accessor, variable_categorizer};
        coarsening.coarsen();

        success = splitting_policy.Generate(m_, strength_policy, variable_categorizer);
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

    AMGSolver<AMGStandardCoarsening, AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

    ASSERT_THAT(amg_solver.amg_levels_.size(), Eq(3));
}

TEST_F(BasicAMGSolverTest, TestExpectedVCycleDepth) {
    Vector b;
    AMGMonitor monitor;
    monitor.direct_solver_threshold = 3;

    AMGSolver<AMGStandardCoarsening, AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

    // test all the cycle indices!
    std::vector<int> cycle_levels;
    cycle_levels.assign(std::cbegin(amg_solver.cycle_scheme_), std::cend(amg_solver.cycle_scheme_));

    ASSERT_THAT(cycle_levels, ElementsAre(0, 1, 2, 1, 0));
}

TEST_F(BasicAMGSolverTest, TestGridComplexity) {
    AMGMonitor monitor;
    monitor.direct_solver_threshold = 3;

    Vector b{m_.cols()};
    AMGSolver<AMGStandardCoarsening, AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};
    ASSERT_FLOAT_EQ(amg_solver.GridComplexity(), 1.66666663f);
}

TEST_F(BasicAMGSolverTest, TestOperatorComplexity) {
    AMGMonitor monitor;
    monitor.direct_solver_threshold = 3;

    Vector b{m_.cols()};
    AMGSolver<AMGStandardCoarsening, AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};
    ASSERT_FLOAT_EQ(amg_solver.OperatorComplexity(), 1.66666663f);
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

    AMGSolver<AMGStandardCoarsening, AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, b, monitor};

//    m_.print();

    Vector x{b.size()};
    std::fill(std::begin(x), std::end(x), 0);
    bool success1;
    std::tie(success1, x) = amg_solver.Solve(x);
    ASSERT_TRUE(success1);

    Vector x_ref{b.size()};
    int iterations;
    std::tie(success1, x_ref, iterations) = SparseLinearSolverLibrary::SparseSOR(m_, b, 1.1, 11000);
    ASSERT_TRUE(success1);

    // compare vectors
    ASSERT_TRUE(SparseLinearSolverUtil::isVectorEqual(x, x_ref, tolerance));
}

TEST_F(BasicAMGSolverTest, TestWSolve) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;
    m_ = stencil.generateMatrix(8 * 8);

    AMGMonitor monitor;
    monitor.direct_solver_threshold = 3;
    monitor.nmax_iterations = 501;
    monitor.nu1 = monitor.nu2 = 1;

    double tolerance = 1E-16;
    monitor.required_tolerance = tolerance;


    Vector b{m_.cols()};
    std::fill(std::begin(b), std::end(b), 1);

    AMGSolver<AMGStandardCoarsening, AMGDirectInterpolationPolicy, AMGWCycle<2>> amg_solver{m_, b, monitor};

    //    m_.print();

    Vector x{b.size()};
    std::fill(std::begin(x), std::end(x), 0);
    bool success1;
    std::tie(success1, x) = amg_solver.Solve(x);
    ASSERT_TRUE(success1);

    Vector x_ref{b.size()};
    int iterations;
    std::tie(success1, x_ref, iterations) = SparseLinearSolverLibrary::SparseSOR(m_, b, 1.1, 11000);
    ASSERT_TRUE(success1);

    // compare vectors
    ASSERT_TRUE(SparseLinearSolverUtil::isVectorEqual(x, x_ref, 4.5 * 1E-5));
}
