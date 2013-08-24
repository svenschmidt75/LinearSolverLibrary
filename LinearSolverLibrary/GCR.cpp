#include "pch.h"

#include "GCR.h"
#include "LinAlg/VectorMath.h"
#include "LinAlg/entities.h"
#include "LinAlg/operators.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

GCR::Return_t
GCR::solve(SparseMatrix2D const & A, Vector const & b, SparseMatrix2D::size_type m, int maxIterations, double tolerance) {
    setUp(m);

    int iteration = 0;

    Vector r(b);

    Vector x(b.size());

    while (iteration <= maxIterations) {
        p.push_back(r);
        for (IMatrix2D::size_type j = 0; j < m && iteration <= maxIterations; ++j, ++iteration) {
            Vector const & pj = p[0];
            Vector Apj = A * pj;
            Ap.push_back(Apj);
            double Apj2 = VectorMath::dotProduct(Apj, Apj);
            double alpha = VectorMath::dotProduct(r, Apj) / Apj2;
            x += alpha * pj;
            r -= alpha * Apj;
            Vector const & Arj = A * r;
            Vector pj1 = r;
            Vector newApj(Apj.size());
            for (IMatrix2D::size_type i = 0; i <= j; ++i) {
                Vector const & Api = Ap[i];
                double Api2 = VectorMath::dotProduct(Api, Api);
                double beta = - VectorMath::dotProduct(Arj, Api) / Api2;
                pj1 += beta * p[i];
                newApj += beta * Api;
            }
            p.push_back(pj1);
            newApj += Arj;
            Ap.push_back(newApj);
        }
        r = b - A * x;
        double normr = VectorMath::norm(r);
        double normb = VectorMath::norm(b);
        double residual = normr / normb;
        if (residual <= tolerance)
            return std::make_tuple(true, x, iteration, residual);
    }
    // scheme did not converge
    return std::make_tuple(false, x, maxIterations, 0);
}

void
GCR::setUp(IMatrix2D::size_type dim) {
    p.reserve(dim);
    Ap.reserve(dim);
}

} // LinearSolverLibrary_NS
