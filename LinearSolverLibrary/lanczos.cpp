#include "pch.h"

#include "lanczos.h"
#include "LinAlg/entities.h"
#include "LinAlg/operators.h"
#include "LinAlg/VectorMath.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

void
Lanczos::init(LinAlg_NS::SparseMatrix2D const & A, Vector const & q0) const {
    A_ = A;
    IMatrix2D::size_type dim = A_.cols() * 10;
    a.resize(dim);
    b.resize(dim);
    q.resize(dim, Vector(dim));
    q[0] = q0;

    // compute 2nd Lanczos vector
    Vector w = A_ * q0;
    double alpha = VectorMath::dotProduct(w, q0);
    w -= alpha * q0;
    double beta = VectorMath::norm(w);
    q[1] = w * (1.0 / beta);

    a[1] = alpha;
    b[1] = beta;

    current_lanczos_vector_index = 2;
}

void
Lanczos::computeNextLanczosVector() const {
//    BOOST_ASSERT_MSG(current_lanczos_vector_index < A_.cols(), "Lanczos::computeNextLanczosVector: Insufficient space");
    Vector const & qn = q[current_lanczos_vector_index - 1];
    double beta = getCurrentBeta();
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

double
Lanczos::getCurrentAlpha() const {
    return a[current_lanczos_vector_index - 1];
}

double
Lanczos::getCurrentBeta() const {
    return b[current_lanczos_vector_index - 1];
}

Vector const &
Lanczos::getPreviousLanczosVector() const {
    return q[current_lanczos_vector_index - 2];
}

double
Lanczos::getPreviousAlpha() const {
    return a[current_lanczos_vector_index - 2];
}

double
Lanczos::getPreviousBeta() const {
    return b[current_lanczos_vector_index - 2];
}

} // LinearSolverLibrary_NS
