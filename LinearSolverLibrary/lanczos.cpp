#include "pch.h"

#include "lanczos.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {

void
Lanczos::init(LinAlg_NS::SparseMatrix2D const & A, Vector const & q0) const {
    A_ = &A;
    IMatrix2D::size_type dim = A_->cols();
    extendCapacity(dim);

    // 1st Lanczos vector
    q.push_back(q0);

    // compute 2nd Lanczos vector
    Vector w = (*A_) * q0;
    double alpha = VectorMath::dotProduct(w, q0);
    w -= alpha * q0;
    double beta = VectorMath::norm(w);
    q.emplace_back((1.0 / beta) * w);

    a.push_back(0);
    a.push_back(alpha);
    b.push_back(0);
    b.push_back(beta);

    current_lanczos_vector_index = 2;
}

void
Lanczos::computeNextLanczosVector() const {
    if (q.capacity() < current_lanczos_vector_index + 1)
        extendCapacity(current_lanczos_vector_index * 2);
    Vector const & qn = q[current_lanczos_vector_index - 1];
    double beta = getCurrentBeta();
    Vector w = (*A_) * qn;
    w -= beta * q[current_lanczos_vector_index - 2];
    double alpha = VectorMath::dotProduct(w, qn);
    w -= alpha * qn;
    beta = VectorMath::norm(w);
    q.emplace_back((1.0 / beta) * w);

    a.push_back(alpha);
    b.push_back(beta);

    current_lanczos_vector_index++;
}

void
Lanczos::extendCapacity(IMatrix2D::size_type new_size) const {
    BOOST_ASSERT_MSG(new_size > q.capacity(), "Lanczos::extendCapacity: Capacity should be bigger than existing one");
    a.reserve(new_size);
    b.reserve(new_size);
    q.reserve(new_size);
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
