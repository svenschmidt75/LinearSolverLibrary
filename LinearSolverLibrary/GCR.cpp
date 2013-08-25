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

    double old_residual = 1.0;

    double normb = VectorMath::norm(b);

    Vector r(b);
    Vector x(b.size());

    std::vector<Vector> p;
    p.reserve(m);
    std::vector<Vector> Ap;
    Ap.reserve(m);

    while (iteration <= maxIterations) {
        p.emplace_back(r);
        Ap.emplace_back(A * r);
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
            p.emplace_back(next_pj);
            next_Apj += Arj;
            Ap.emplace_back(next_Apj);

            double normr = VectorMath::norm(r);
            double residual = normr / normb;
            if (residual <= tolerance)
                return std::make_tuple(true, x, iteration, residual);

            /* The numerical scheme might get stuck, i.e. no improvements
             * will be made w.r.t. the residual. In this case, there is
             * no point in keeping the iteration running. This might happen
             * for matrices with very high condition number.
             */
            double tmp = std::fabs(std::log(residual / old_residual));
            if (tmp < tolerance)
                return std::make_tuple(true, x, iteration, residual);

            if (!(j % 20))
                old_residual = residual;
        }
        // restart
        p.clear();
        Ap.clear();
    }
    // scheme did not converge
    return std::make_tuple(false, x, maxIterations, 0);
}

} // LinearSolverLibrary_NS
