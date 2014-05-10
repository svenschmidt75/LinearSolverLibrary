#include "pch.h"

#include "MultigridTutorialExampleTestFramework.h"

using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


namespace {

    class HighResTimer {
    public:
        HighResTimer() : start_(boost::chrono::high_resolution_clock::now()) {}
        ~HighResTimer() {
            auto end = boost::chrono::high_resolution_clock::now();
            auto d = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start_);
            std::cout << std::endl << "Duration: " << d << std::endl;
        }

    private:
        boost::chrono::steady_clock::time_point start_;
    };

}

TEST(MultigridTutorialExampleTestFrameworkAMGTest, TestAMGHierarchyBuilderWithCoarseningNotTooLastLevel) {
    int mesh_size = 4;
    auto framework = MultigridTutorialExampleTestFramework{mesh_size};
    framework.InitializeWithStencil2();

    AMGMonitor monitor;

    // Instruct the AMGHierarchyBuilder to coarse until the Galerkin operator is a 1x1 matrix.
    // With InitializeWithStencil2, coarsening cannot advance past 2x2.
    monitor.direct_solver_threshold = 1;
    monitor.nmax_iterations = 1001;
    monitor.nu1 = monitor.nu2 = 1;
    monitor.verbosity = 2;

    AMGHierarchyBuilder<AMGDirectInterpolationPolicy, AMGCThenFRelaxationPolicy> builder{monitor};
    auto amg_levels = builder.build(framework.m_);

    ASSERT_EQ(3, amg_levels.size());
}

TEST(MultigridTutorialExampleTestFrameworkAMGTest, TestNEquals16Case) {
    int mesh_size = 4;
    auto framework = MultigridTutorialExampleTestFramework{mesh_size};
    //framework.InitializeWithStencil1();
    framework.InitializeWithStencil2();
    //framework.InitializeWithStencil3();
    //framework.InitializeWithStencil4();

    AMGMonitor monitor;
    monitor.direct_solver_threshold = 4;
    monitor.nmax_iterations = 1001;
    monitor.nu1 = monitor.nu2 = 1;
    monitor.verbosity = 2;

    double tolerance = 1E-17;
    monitor.required_tolerance = tolerance;

    auto sol = framework.DirectSolve();
    double error = framework.L2Error(sol);

    Vector amg_solution_vector;
    {
        HighResTimer t;
        amg_solution_vector = framework.SolveWithAMG(monitor);
    }

    error = framework.LinfError(amg_solution_vector);
    std::cout << "AMG: " << error << std::endl;

    ASSERT_DOUBLE_EQ(error, 0.013843463062068982);
}

#ifdef INTEGRATION_TEST

TEST(MultigridTutorialExampleTestFrameworkAMGTest, TestThatErrorReducesBy1Over4WhenMeshSizeIncreasesBy1Over2) {
    double error2[6];
    double errorInf[6];

    for (int factor = 0; factor < 6; ++factor) {
        unsigned short mesh_size = static_cast<unsigned short>(std::pow(2.0, factor + 2));
        auto framework = MultigridTutorialExampleTestFramework{mesh_size};
        framework.InitializeWithStencil1();
        //framework.InitializeWithStencil2();
        //framework.InitializeWithStencil3();
        //framework.InitializeWithStencil4();

        Vector solution_cg = framework.SolveWithCG();

        double l2_error = framework.L2Error(solution_cg);
        double linf_error = framework.LinfError(solution_cg);
        error2[factor] = l2_error;
        errorInf[factor] = linf_error;
    }

    // l2_error[i] / l2_error{i - 1] -> 4

    for (int factor = 1; factor < 6; ++factor) {
        double delta = error2[factor - 1] / error2[factor];
        std::cout << delta << std::endl;
    }

    std::cout << std::endl;

    for (int factor = 1; factor < 6; ++factor) {
        double delta = errorInf[factor - 1] / errorInf[factor];
        std::cout << delta << std::endl;
    }
}

#endif
