#include "pch.h"

#include "lanczos.h"
#include "LinAlg/entities.h"
#include "LinAlg/operators.h"
#include "LinAlg/VectorMath.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

Lanczos::Lanczos(LinAlg_NS::SparseMatrix2D const & A, Vector const & q0) : A_(A) {
    IMatrix2D::size_type dim = A.cols();
    a.resize(dim);
    b.resize(dim);
    q.resize(dim, Vector(dim));
    q[0] = q0;

    // compute 2nd Lanczos vector
    Vector w = A * q0;
    double alpha = VectorMath::dotProduct(w, q0);
    w -= alpha * q0;
    double beta = VectorMath::norm(w);
    q[1] = w * (1.0 / beta);

    current_lanczos_vector_index = 2;
}

void
Lanczos::computeNextLanczosVector() const {
    BOOST_ASSERT_MSG(current_lanczos_vector_index < A_.cols(), "Lanczos::computeNextLanczosVector: Insufficient space");
    Vector const & qn = q[current_lanczos_vector_index - 1];
    double beta = getLastBeta();
    Vector w = A_ * qn;
    w -= beta * q[current_lanczos_vector_index - 2];
    double alpha = VectorMath::dotProduct(w, qn);
    w -= alpha * qn;
    beta = VectorMath::norm(w);
    q[current_lanczos_vector_index] = w * (1.0 / beta);

    a[current_lanczos_vector_index] = alpha;
    b[current_lanczos_vector_index] = beta;

    current_lanczos_vector_index++;
}

Vector const &
Lanczos::getLastLanczosVector() const {
    return q[current_lanczos_vector_index - 1];
}

double
Lanczos::getLastAlpha() const {
    return a[current_lanczos_vector_index - 1];
}

double
Lanczos::getLastBeta() const {
    return b[current_lanczos_vector_index - 1];
}

} // LinearSolverLibrary_NS
