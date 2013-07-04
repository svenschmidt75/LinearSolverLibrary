#include "pch.h"

#include "lanczosPRO.h"
#include "LinAlg/entities.h"
#include "LinAlg/operators.h"
#include "LinAlg/VectorMath.h"


using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {
    
namespace {

    void printLanczosVectorsOrthogonal(std::vector<Vector> const & q, IMatrix2D::size_type size) {
        /* The best we can hope for in orthogonality of the Lanczos vectors
         * is <q_i, q_j> = eps if they are orthogonal.
         * Here, we compute the exponent of how much the orthogonality deviates
         * from eps.
         * Example: <q_i, q_j> = 10^{-12}
         * Result: 4, i.e. -12 + 16 (from eps) = 4
         */
        double const machine_eps = std::numeric_limits<double>::epsilon();
        std::cout << std::endl;
        for (IMatrix2D::size_type i = 0; i < size - 1; ++i) {
            double angle = VectorMath::dotProduct(q[i], q[size - 1]);
            double deviation = std::fabs(angle / machine_eps);
            deviation = std::log10(deviation);
            int int_deviation = static_cast<int>(boost::math::round(deviation));
            std::cout << int_deviation << " ";
        }
    }
}

void
LanczosPRO::init(LinAlg_NS::SparseMatrix2D const & A, Vector const & q0) const {
    A_ = A;
    IMatrix2D::size_type dim = A_.cols() * 10;
    a.resize(dim);
    b.resize(dim);
    q.resize(dim, Vector(dim));
    w1.resize(dim);
    w2.resize(dim);
    w3.resize(dim);

    // 1st Lanczos vector
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
LanczosPRO::computeNextLanczosVector() const {
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

    monitorOrthogonality();
}

double
LanczosPRO::getCurrentAlpha() const {
    return a[current_lanczos_vector_index - 1];
}

double
LanczosPRO::getCurrentBeta() const {
    return b[current_lanczos_vector_index - 1];
}

Vector const &
LanczosPRO::getPreviousLanczosVector() const {
    return q[current_lanczos_vector_index - 2];
}

double
LanczosPRO::getPreviousAlpha() const {
    return a[current_lanczos_vector_index - 2];
}

double
LanczosPRO::getPreviousBeta() const {
    return b[current_lanczos_vector_index - 2];
}

void
LanczosPRO::monitorOrthogonality() const {
    double const eps = std::numeric_limits<double>::epsilon();
    double eps2 = std::sqrt(eps);
    eps2 = eps2;
    auto i = current_lanczos_vector_index - 2;
    for (IMatrix2D::size_type j = 1; j <= i - 1; ++j) {
        double beta_ip1 = b[i + 1];

        double beta_jp1 = b[j + 1];
        double omega_i_jp1 = i == j + 1 ? 1.0 : w2[j + 1];
        double term1 = beta_jp1 * omega_i_jp1 / beta_ip1;

        double aa = a[j + 1] - a[i + 1];
        double omega_i_j = i == j ? 1.0 : w2[j];
        double term2 = aa * omega_i_j / beta_ip1;

        double beta_j = b[j];
        double omega_i_jm1 = i == j - 1 ? 1.0 : (j - 1 == 0 ? 0 : w2[j - 1]);
        double term3 = beta_j * omega_i_jm1 / beta_ip1;

        double beta_i = b[i];
        double omega_j_im1 = j == i - 1 ? 1.0 : (i - 1 == 0 ? 0 : w1[j]);
        double term4 = beta_i * omega_j_im1 / beta_ip1;

        double sum = (beta_jp1 * omega_i_jp1 + aa * omega_i_j + beta_j * omega_i_jm1 - beta_i * omega_j_im1) / beta_ip1;
        sum = term1 + term2 + term3 - term4;
        w3[j] = sum;

        double angle = std::sqrt(VectorMath::dotProduct(q[i + 1], q[j]));
        angle = angle;
    }
    w3[i] = A_.cols() * 0.5 * eps;

    // TODO: Use indices instead of copying
    w1 = w2;
    w2 = w3;
}

} // LinearSolverLibrary_NS
