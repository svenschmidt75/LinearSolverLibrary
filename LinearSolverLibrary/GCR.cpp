#include "pch.h"

#include "GCR.h"
#include "LinAlg/VectorMath.h"
#include "LinAlg/entities.h"
#include "LinAlg/operators.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

GCR::Return_t
GCR::solve(SparseMatrix2D const & A, Vector const & b, SparseMatrix2D::size_type m, int maxIterations, double tolerance) {
    int iteration = 0;

    Vector r(b);
    Vector x(b.size());
    std::vector<Vector> p;
    p.reserve(b.size());
    std::vector<Vector> Ap;
    Ap.reserve(b.size());

    while (iteration <= maxIterations) {
        p.push_back(r);
        Ap.push_back(A * r);
        for (IMatrix2D::size_type j = 0; j < m && iteration <= maxIterations; ++j, ++iteration) {
            Vector const & pj = p[j];
            Vector const & Apj = Ap[j];
            double Apj2 = VectorMath::dotProduct(Apj, Apj);
            double alpha = VectorMath::dotProduct(r, Apj) / Apj2;
            x += alpha * pj;
            r -= alpha * Apj;
            Vector Arj = A * r;
            Vector next_pj = r;
            Vector next_Apj(Apj.size());
            for (IMatrix2D::size_type i = 0; i <= j; ++i) {
                Vector const & Api = Ap[i];
                double Api2 = VectorMath::dotProduct(Api, Api);
                double beta = - VectorMath::dotProduct(Arj, Api) / Api2;
                next_pj += beta * p[i];
                next_Apj += beta * Api;
            }
            p.push_back(next_pj);
            next_Apj += Arj;
            Ap.push_back(next_Apj);

            r = b - A * x;
            double normr = VectorMath::norm(r);
            double normb = VectorMath::norm(b);
            double residual = normr / normb;
            if (residual <= tolerance)
                return std::make_tuple(true, x, iteration, residual);
        }

        // restart
        p.clear();
        Ap.clear();
    }
    // scheme did not converge
    return std::make_tuple(false, x, maxIterations, 0);
}

} // LinearSolverLibrary_NS
