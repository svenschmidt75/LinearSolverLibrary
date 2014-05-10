#include "pch.h"

#include "MultigridTutorialExampleTestFramework.h"

#include "LinearSolverLibrary/AMGSolver.hpp"

#include "LinAlg/MatrixStencil.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


MultigridTutorialExampleTestFramework::MultigridTutorialExampleTestFramework() {}

MultigridTutorialExampleTestFramework::MultigridTutorialExampleTestFramework(int mesh_size)
    :
    mesh_size_{mesh_size} {}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil1() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
    h2_ = 1.0 / ((mesh_size_ + 1) * (mesh_size_ + 1));
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil2() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1,  0, -1,
                0,  4,  0,
               -1,  0, -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
    h2_ = 2.0 / ((mesh_size_ + 1) * (mesh_size_ + 1));
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil3() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
    h2_ = 3.0 / ((mesh_size_ + 1) * (mesh_size_ + 1));
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil4() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -4,  -1,
               -4,  20, -4,
               -1, -4,  -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
    h2_ = 6.0 / ((mesh_size_ + 1) * (mesh_size_ + 1));
}

double
MultigridTutorialExampleTestFramework::GetX(int i) const {
    common_NS::reporting::checkUppderBound(i, mesh_size_ - 1);
    common_NS::reporting::checkLowerBound(i, 0);
    double h = 1.0 / (mesh_size_ + 1);
    return (1 + i) * h;
}

double
MultigridTutorialExampleTestFramework::GetY(int j) const {
    common_NS::reporting::checkUppderBound(j, mesh_size_ - 1);
    common_NS::reporting::checkLowerBound(j, 0);
    double h = 1.0 / (mesh_size_ + 1);
    return (1 + j) * h;
}

double
MultigridTutorialExampleTestFramework::f(double x, double y) const {
    double term1 = (1.0 - 6.0 * x * x) * y * y * (1.0 - y * y);
    double term2 = (1.0 - 6.0 * y * y) * x * x * (1.0 - x * x);
    double result = 2.0 * (term1 + term2);
    result = result * h2_;
    return result;
}

double
MultigridTutorialExampleTestFramework::Solution(double x, double y) const {
    double x2 = x * x;
    double y2 = y * y;
    double term1 = 1.0 - x2;
    double term2 = y2 - 1.0;
    double result = x2 * term1 * y2 * term2;
    return result;
}

Vector
MultigridTutorialExampleTestFramework::DirectSolve() const {
    Matrix2D dense = helper::SparseToDense(m_);
    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(dense);
    common_NS::reporting::checkConditional(success, "MultigridTutorialExampleTestFramework::DirectSolve: LU decomposition failed");
    Vector rhs = CreateRHS();
    Vector x = ludecomp.solve(rhs);

    // consistency check
    Vector tmp = m_ * x;
    double error = VectorMath::L2Error(rhs, tmp);
    if (error > 1E-14)
        throw std::logic_error("MultigridTutorialExampleTestFramework::DirectSolve: Consistency check failed");

    return x;
}

Vector
MultigridTutorialExampleTestFramework::SolveWithCG() const {
    Vector rhs = CreateRHS();
    Vector x{rhs.size()};
    SparseMatrix2D::size_type iterations;
    double tol;
    bool success;
    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m_, rhs, 10000);
    common_NS::reporting::checkConditional(success, "MultigridTutorialExampleTestFramework::SolveWithCG: CG did not converge");
    return x;
}

Vector
MultigridTutorialExampleTestFramework::SolveWithAMG(AMGMonitor & monitor) const {
    Vector rhs = CreateRHS();
    Vector x{rhs.size()};

    AMGSolver<AMGDirectInterpolationPolicy, AMGVCycle> amg_solver{m_, rhs, monitor};
//    m_.print();

    bool success;
    std::tie(success, x) = amg_solver.Solve(x);
    common_NS::reporting::checkConditional(success, "MultigridTutorialExampleTestFramework::SolveWithAMG: AMG did not converge");
    return x;
}

double
MultigridTutorialExampleTestFramework::LinfError(Vector const & in) const {
    Vector solution = CreateExactSolutionVector();
    return VectorMath::LinfError(in, solution);
}

double
MultigridTutorialExampleTestFramework::L2Error(Vector const & in) const {
    Vector solution = CreateExactSolutionVector();
    return VectorMath::L2Error(in, solution);
}

Vector
MultigridTutorialExampleTestFramework::CreateRHS() const {
    return ComputeVector([this](double x, double y) -> double {
        return f(x, y);
    });
}

Vector
MultigridTutorialExampleTestFramework::CreateExactSolutionVector() const {
    return ComputeVector([this](double x, double y) -> double {
        return Solution(x, y);
    });
}

Vector
MultigridTutorialExampleTestFramework::ComputeVector(std::function<double (double, double)> const & func) const {
    Vector solution{static_cast<Vector::size_type>(mesh_size_) * mesh_size_};
    for (int j = 0; j < mesh_size_; ++j) {
        double y = GetY(j);
        for (int i = 0; i < mesh_size_; ++i) {
            double x = GetX(i);
            double value = func(x, y);
            int index = j * mesh_size_ + i;
            solution(index) = value;
        }
    }
    return solution;
}

double
MultigridTutorialExampleTestFramework::Factor() const {
    return h2_;
}
