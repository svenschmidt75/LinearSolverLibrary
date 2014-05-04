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


TEST(MultigridTutorialExampleTestFrameworkAMGTest, TestNEquals16Case) {
    int mesh_size = 32;
    auto framework = MultigridTutorialExampleTestFramework{mesh_size};
    framework.InitializeWithStencil1();
    //framework.InitializeWithStencil2();
    //framework.InitializeWithStencil3();
    //framework.InitializeWithStencil4();

    AMGMonitor monitor;
    monitor.direct_solver_threshold = 2;
    monitor.nmax_iterations = 1001;
    monitor.nu1 = monitor.nu2 = 1;
    monitor.verbosity = 1;

    double tolerance = 1E-17;
    monitor.required_tolerance = tolerance;

    Vector amg_solution_vector;
    {
        HighResTimer t;
        //amg_solution_vector = framework.SolveWithAMG(monitor);
    }

    auto sol = framework.DirectSolve();
    double error = framework.L2Error(sol);
    std::cout << "DirectSolve: " << error << std::endl;

    //Vector s = framework.SolveWithCG();
    //error = framework.L2Error(s);
    //std::cout << "CG: " << error << std::endl;


    //error = framework.L2Error(amg_solution_vector);
    //std::cout << "AMG: " << error << std::endl;

    ASSERT_THAT(error, DoubleNear(0.0, 1E-13));
}
