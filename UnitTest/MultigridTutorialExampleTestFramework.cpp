#include "pch.h"

#include "MultigridTutorialExampleTestFramework.h"


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
//    factor_ = 1.0 / (mesh_size_ * mesh_size_);
    factor_ = 1.0 / ((mesh_size_ + 1) * (mesh_size_ + 1));
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil2() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1,  0, -1,
                0,  4,  0,
               -1,  0, -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
//    factor_ = 1.0 / (2.0 * mesh_size_ * mesh_size_);
    factor_ = 1.0 / (2.0 * (mesh_size_ + 1) * (mesh_size_ + 1));
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil3() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
//    factor_ = 1.0 / (8.0 * mesh_size_ * mesh_size_);
    factor_ = 1.0 / (8.0 * (mesh_size_ + 1) * (mesh_size_ + 1));
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil4() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -4, -1,
               -4, 20, -4,
               -1, -4, -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
//    factor_ = 1.0 / (20.0 * mesh_size_ * mesh_size_);
    factor_ = 1.0 / (20.0 * (mesh_size_ + 1) * (mesh_size_ + 1));
}

double
MultigridTutorialExampleTestFramework::GetX(int i) const {
    common_NS::reporting::checkUppderBound(i, mesh_size_ - 1);
    common_NS::reporting::checkLowerBound(i, 0);
    double h = 1.0 / (mesh_size_ + 1);
    return (i + 1) * h;
}

double
MultigridTutorialExampleTestFramework::GetY(int j) const {
    common_NS::reporting::checkUppderBound(j, mesh_size_ - 1);
    common_NS::reporting::checkLowerBound(j, 0);
    double h = 1.0 / (mesh_size_ + 1);
    return (j + 1) * h;
}

double
MultigridTutorialExampleTestFramework::f(double x, double y) const {
    double term1 = (1.0 - 6.0 * x * x) * y * y * (1.0 - y * y);
    double term2 = (1.0 - 6.0 * y * y) * x * x * (1.0 - x * x);
    double result = 2.0 * (term1 + term2);
    return result * factor_;
}

double
MultigridTutorialExampleTestFramework::Solution(double x, double y) const {
    double result = x * x * ( 1.0 - x * x) * y * y * (y * y - 1.0);
    return result;
}

Vector
MultigridTutorialExampleTestFramework::DirectSolve() const {
    Matrix2D dense = helper::SparseToDense(m_);
    dense.print();
    LUDecomposition ludecomp;
    bool success = ludecomp.decompose(dense);
    common_NS::reporting::checkConditional(success, "MultigridTutorialExampleTestFramework::DirectSolve: LU decomposition failed");
    Vector rhs = CreateRHS();
    Vector x = ludecomp.solve(rhs);
    return x;
}

double
MultigridTutorialExampleTestFramework::LinfError(Vector const & in) const {
    Vector solution = CreateExactSolutionVector();
    return VectorMath::LinfError(in, solution);
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
    return factor_;
}
