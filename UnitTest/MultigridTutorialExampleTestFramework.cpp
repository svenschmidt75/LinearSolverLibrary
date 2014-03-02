#include "pch.h"

#include "MultigridTutorialExampleTestFramework.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/LUDecomposition.h"


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
    factor_ = 1.0 / (mesh_size_ * mesh_size_);
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil2() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1,  0, -1,
                0,  4,  0,
               -1,  0, -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
    factor_ = 1.0 / (2.0 * mesh_size_ * mesh_size_);
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil3() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
    factor_ = 1.0 / (8.0 * mesh_size_ * mesh_size_);
}

void
MultigridTutorialExampleTestFramework::InitializeWithStencil4() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -4, -1,
               -4, 20, -4,
               -1, -4, -1;
    m_ = stencil.generateMatrix(mesh_size_ * mesh_size_);
    factor_ = 1.0 / (20.0 * mesh_size_ * mesh_size_);
}

double
MultigridTutorialExampleTestFramework::GetX(int i) const {
    common_NS::reporting::checkUppderBound(i, mesh_size_ - 1);
    common_NS::reporting::checkLowerBound(i, 0);
    double offset = 1.0 / (mesh_size_ + 1);
    return (i + 1) * offset;
}

double
MultigridTutorialExampleTestFramework::GetY(int j) const {
    common_NS::reporting::checkUppderBound(j, mesh_size_ - 1);
    common_NS::reporting::checkLowerBound(j, 0);
    double offset = 1.0 / (mesh_size_ + 1);
    return (j + 1) * offset;
}

double
MultigridTutorialExampleTestFramework::f(double x, double y) const {
    double term11 = 1.0 - 6.0 * x * x;
    double term12 = term11 * y * y * (1.0 - y * y);
    double term21 = 1.0 - 6.0 * y * y;
    double term22 = term21 * x * x * (1.0 - x * x);
    double result = 2.0 * (term12 + term22);
    return result * factor_;
}

double
MultigridTutorialExampleTestFramework::Solution(double x, double y) const {
    double term11 = x * x;
    double term12 = y * y;
    double term21 = term11 - term11 * term11;
    double term22 = term12 * term12 - term12;
    return term21 * term22;
}

Vector
MultigridTutorialExampleTestFramework::DirectSolve() const {
    LUDecomposition ludecomp;
    Matrix2D dense = helper::SparseToDense(m_);
    bool success = ludecomp.decompose(dense);
    common_NS::reporting::checkConditional(success, "MultigridTutorialExampleTestFramework::DirectSolve: LU decomposition failed");
    Vector rhs = CreateRHS();
    Vector x = ludecomp.solve(rhs);
    return x;
}

Vector
MultigridTutorialExampleTestFramework::CreateRHS() const {
    Vector rhs{static_cast<Vector::size_type>(mesh_size_) * mesh_size_};
    for (int i = 0; i < mesh_size_; ++i) {
        for (int j = 0; j < mesh_size_; ++j) {
            double x = GetX(i);
            double y = GetX(j);
            double value = f(x, y);
            int index = i * mesh_size_ + j;
            rhs(index) = value;
        }
    }
    return rhs;
}

double
MultigridTutorialExampleTestFramework::Factor() const {
    return factor_;
}
