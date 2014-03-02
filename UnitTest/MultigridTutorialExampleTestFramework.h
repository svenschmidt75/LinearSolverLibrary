/*
 * Name  : MultigridTutorialExampleTestFramework
 * Path  :
 * Use   :
 * Author: Sven Schmidt
 * Date  : 03/01/2014
 */
#pragma once


class MultigridTutorialExampleTestFramework {
public:
    MultigridTutorialExampleTestFramework();
    MultigridTutorialExampleTestFramework(int mesh_size);

    void InitializeWithStencil1();
    void InitializeWithStencil2();
    void InitializeWithStencil3();
    void InitializeWithStencil4();

    double GetX(int i) const;
    double GetY(int j) const;

    double            f(double x, double y) const;
    double            Solution(double x, double y) const;
    LinAlg_NS::Vector DirectSolve() const;
    double            LinfError(Vector const & in) const;

    // to facilitate unit testing
    double            Factor() const;
    LinAlg_NS::Vector CreateRHS() const;
    LinAlg_NS::Vector CreateExactSolutionVector() const;
    LinAlg_NS::Vector ComputeVector(std::function<double(double, double)> const & func) const;

public:
    LinAlg_NS::SparseMatrix2D m_;
    int                       mesh_size_;
    double                    factor_;
};