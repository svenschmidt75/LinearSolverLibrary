/*
 * Name  : MultigridTutorialExampleTestFramework
 * Path  :
 * Use   :
 * Author: Sven Schmidt
 * Date  : 03/01/2014
 */
#pragma once


class MultigridTutorialExampleTestFramework {


    friend class MultigridTutorialExampleTestFrameworkTest;


public:
    MultigridTutorialExampleTestFramework();
    MultigridTutorialExampleTestFramework(int mesh_size);

    void InitializeWithStencil1();
    void InitializeWithStencil2();
    void InitializeWithStencil3();
    void InitializeWithStencil4();

    double GetX(int i) const;
    double GetY(int j) const;

    double f(double x, double y) const;

    // to facilitate unit testing
    double Factor() const;

private:
    LinAlg_NS::SparseMatrix2D m_;
    int                       mesh_size_;
    double                    factor_;
};