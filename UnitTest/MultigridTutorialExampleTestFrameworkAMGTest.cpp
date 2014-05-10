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

TEST(MultigridTutorialExampleTestFrameworkAMGTest, DISABLED_TestNEquals16Case) {
    int mesh_size = 8;
    auto framework = MultigridTutorialExampleTestFramework{mesh_size};
    //framework.InitializeWithStencil1();
    framework.InitializeWithStencil2();
    //framework.InitializeWithStencil3();
    //framework.InitializeWithStencil4();

    AMGMonitor monitor;
    monitor.direct_solver_threshold = 4;
    monitor.nmax_iterations = 1001;
    monitor.nu1 = monitor.nu2 = 1;
    monitor.verbosity = 1;

    double tolerance = 1E-17;
    monitor.required_tolerance = tolerance;

    Vector amg_solution_vector;
    {
        HighResTimer t;
        amg_solution_vector = framework.SolveWithAMG(monitor);
    }

    auto sol = framework.DirectSolve();
    double error = framework.L2Error(sol);
    std::cout << "DirectSolve: " << error << std::endl;

    Vector s = framework.SolveWithCG();
    error = framework.LinfError(s);
    std::cout << "CG: " << error << std::endl;


    error = framework.LinfError(amg_solution_vector);
    std::cout << "AMG: " << error << std::endl;

    ASSERT_THAT(error, DoubleNear(0.0, 1E-13));
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
