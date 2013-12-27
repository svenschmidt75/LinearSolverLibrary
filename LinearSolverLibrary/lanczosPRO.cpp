#include "pch.h"

#include "lanczosPRO.h"


using namespace LinAlg_NS;


#define WRITE_TO_CONSOLE 0


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
#if WRITE_TO_CONSOLE
        double const machine_eps = std::numeric_limits<double>::epsilon();
        std::cout << std::endl;
        for (IMatrix2D::size_type i = 0; i < size - 1; ++i) {
            double angle = VectorMath::dotProduct(q[i], q[size - 1]);
            int int_deviation = 0;
            if (angle) {
                double deviation = std::fabs(angle / machine_eps);
                deviation = std::log10(deviation);
                if (deviation < 0)
                    // precision cannot possibly be better than eps
                    deviation = 0;
                int_deviation = static_cast<int>(boost::math::round(deviation));
            }
            std::cout << int_deviation << " ";
        }
#else
        size = size;
        q.size();
#endif
    }
}

void
LanczosPRO::init(LinAlg_NS::SparseMatrix2D const & A, Vector const & q0) const {
    A_ = &A;
    IMatrix2D::size_type dim = A_->cols() + 1;

    /* Use
     * 1. reserve instead of resize, so not all the vectors have to be default-created
     *    immediately
     * 2. use push_back
     */
    extendCapacity(dim);
    w1.resize(dim);
    w2.resize(dim);
    w3.resize(dim);

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
    numer_of_reorthogonalizations = 0;

    force_reorthogonalization = false;

    initializeOmega();

    monitorOrthogonality();
}

void
LanczosPRO::initializeOmega() const {
    omega1 = [this]() -> std::vector<double> & {
        return w1;
    };

    omega2 = [this]() -> std::vector<double> & {
        return w2;
    };

    omega3 = [this]() -> std::vector<double> & {
        return w3;
    };
}

void
LanczosPRO::computeNextLanczosVector() const {
    BOOST_ASSERT_MSG(current_lanczos_vector_index < A_->cols() + 1, "Lanczos::computeNextLanczosVector: Insufficient space");
    Vector const & qn = q[current_lanczos_vector_index - 1];
    double beta = getCurrentBeta();
    Vector w = (*A_) * qn;
    w -= beta * q[current_lanczos_vector_index - 2];

    // make w orthogonal to qn
    double alpha = VectorMath::dotProduct(w, qn);
    w -= alpha * qn;

    beta = VectorMath::norm(w);
    q.emplace_back((1.0 / beta) * w);

    a.push_back(alpha);
    b.push_back(beta);

    current_lanczos_vector_index++;

    monitorOrthogonality();
}

void
LanczosPRO::extendCapacity(IMatrix2D::size_type new_size) const {
    BOOST_ASSERT_MSG(new_size > q.capacity(), "LanczosPRO::extendCapacity: Capacity should be bigger than existing one");
    a.reserve(new_size);
    b.reserve(new_size);
    q.reserve(new_size);
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

    if (force_reorthogonalization) {
        // we orthogonalize against the same Lanczos vectors
        // as in the previous iteration
        reorthogonalizeLanczosVector(current_lanczos_vector_index - 1);
        force_reorthogonalization = false;
    }

    auto i = current_lanczos_vector_index - 1;
    if (i > 1) {
        double theta = computeLanczosNorm();
        double beta_ip1 = b[i];
        for (IMatrix2D::size_type j = 0; j < i - 1; ++j) {
            double beta_jp1 = b[j + 1];
            double omega_i_jp1 = i - 1 == j + 1 ? 1.0 : omega2()[j + 1];
            double term1 = beta_jp1 * omega_i_jp1 / beta_ip1;

            double aa = a[j + 1] - a[i];
            double omega_i_j = omega2()[j];
            double term2 = aa * omega_i_j / beta_ip1;

            double beta_j = 0;
            double omega_i_jm1 = 0;
            double term3 = 0;
            if (j) {
                beta_j = b[j];
                omega_i_jm1 = omega2()[j - 1];
                term3 = beta_j * omega_i_jm1 / beta_ip1;
            }

            double beta_i = b[i - 1];
            double omega_j_im1 = j + 1 == i - 1 ? 1.0 : omega1()[j];
            double term4 = beta_i * omega_j_im1 / beta_ip1;

            double term5 = theta / beta_ip1;

            double sum = (beta_jp1 * omega_i_jp1 + aa * omega_i_j + beta_j * omega_i_jm1 - beta_i * omega_j_im1 + theta) / beta_ip1;
            sum = term1 + term2 + term3 - term4 + term5;
            omega3()[j] = sum;

            double angle = VectorMath::dotProduct(q[i], q[j]);

#if WRITE_TO_CONSOLE
            std::cout << std::endl << "(" << i << "," << j << "): is/true: " << sum << ":" << angle;
#endif

            if (std::fabs(angle / sum) > 1E2) {
                std::cout << "LanczosPRO::monitorOrthogonality: Value too optimistic" << std::endl;
            }

            angle = angle;
        }
    }

    // q_{i+1} is per construction (Lanczos) orthogonal to q_{i}
    omega3()[i - 1] = std::sqrt(A_->cols()) * 0.5 * eps;

    bool reorthogonalize = checkForReorthogonalization(i);
    if (reorthogonalize) {
        numer_of_reorthogonalizations++;
#if WRITE_TO_CONSOLE
        std::cout << std::endl << "reorthogonalization " << numer_of_reorthogonalizations;
#endif

        printLanczosVectorsOrthogonal(q, current_lanczos_vector_index);

        findLanczosVectorsToReorthogonalizeAgainst(current_lanczos_vector_index - 1);
        reorthogonalizeLanczosVector(current_lanczos_vector_index - 1);

        force_reorthogonalization = true;
    }

    rotateOmega();
}

void
LanczosPRO::rotateOmega() const {
    omega1 = omega2;
    omega2 = omega3;
}

bool
LanczosPRO::checkForReorthogonalization(IMatrix2D::size_type index) const {
    double const eps = std::numeric_limits<double>::epsilon();
    double const eps2 = std::sqrt(eps);
    for (IMatrix2D::size_type j = 0; j < index; ++j) {
        if (std::fabs(omega3()[j]) > eps2)
            return true;
    }
    return false;
}

void
LanczosPRO::findLanczosVectorsToReorthogonalizeAgainst(IMatrix2D::size_type index) const {
    double const eps = std::numeric_limits<double>::epsilon();
    double const eps2 = std::pow(eps, 3.0 / 4.0);
    indices.resize(index);
    for (IMatrix2D::size_type j = 0; j < index; ++j) {
        double value = std::fabs(omega3()[j]);
        indices[j] = value > eps2;
    }
}

void
LanczosPRO::reorthogonalizeLanczosVector(IMatrix2D::size_type index) const {
    double const eps = std::numeric_limits<double>::epsilon();
    Vector & q_prev = q[index];
    printLanczosVectorsOrthogonal(q, index + 1);
    for (IMatrix2D::size_type i = 0; i < indices.size(); ++i) {
        decltype(i) lanczos_index = indices[i];
        if (lanczos_index) {
            double proj = VectorMath::dotProduct(q_prev, q[i]);
            q_prev -= proj * q[i];
            omega3()[i] = std::sqrt(A_->cols()) * 0.5 * eps;
        }
    }
    // the normalization makes no real difference it seems
//     double norm = VectorMath::norm(q_prev);
//     q_prev = q_prev * (1.0 / norm);
    printLanczosVectorsOrthogonal(q, index + 1);
}


double
LanczosPRO::computeLanczosNorm() const {
    /* Compute norm of matrix A via the symmetric matrix T.
     * Uses Gershgorin's circle theorem to estimate the largest
     * eigenvalue of T.
     */
    double g0;
    if (current_lanczos_vector_index == 3) {
        // estimate T_{2}
        g0_prev = 0;
        g2 = 0;

        double alpha1 = a[1];
        double alpha2 = a[2];
        double beta2 = b[1];
        double scale = std::max(std::fabs(alpha1), std::fabs(alpha2));
        scale = std::max(std::fabs(beta2), scale);

        alpha1 /= scale;
        alpha2 /= scale;
        beta2 /= scale;

        double T00 = alpha1 * alpha1 + beta2 * beta2;
        double T01 = (alpha1 + alpha2) * beta2;
        double T10 = T01;
        double T11 = alpha2 * alpha2 + beta2 * beta2;

        double fac = 1.01 * scale;
        double fac2 = fac * fac;

        // 1st and 2nd row of T_{1}
        g1 = T00 + std::fabs(T01);
        g1 *= fac2;
        g0 = T11 + std::fabs(T10);
        g0 *= fac2;
    }

    // this uses Gershgorin's circle theorem
    else if (current_lanczos_vector_index == 4) {
        // estimate T_{3}
        double alpha1 = a[1];
        double alpha2 = a[2];
        double alpha3 = a[3];
        double beta2 = b[1];
        double beta3 = b[2];

        double scale = std::max(std::fabs(alpha1), std::fabs(alpha2));
        scale = std::max(std::fabs(alpha3), scale);
        scale = std::max(std::fabs(beta2), scale);
        scale = std::max(std::fabs(beta3), scale);

        alpha1 /= scale;
        alpha2 /= scale;
        alpha3 /= scale;
        beta2 /= scale;
        beta3 /= scale;

        double T00 = alpha1 * alpha1 + beta2 * beta2;
        double T01 = (alpha1 + alpha2) * beta2;
        double T02 = beta2 * beta3;
        double T10 = T01;
        double T11 = beta2 * beta2 + alpha2 * alpha2 + beta3 * beta3;
        double T12 = (alpha2 + alpha3) * beta3;
        double T20 = T02;
        double T21 = T12;
        double T22 = alpha3 * alpha3 + beta3 * beta3;

        double fac2 = scale * scale;

        g2 = T00 + std::fabs(T01) + std::fabs(T02);
        g2 *= fac2;
        g1 = T11 + std::fabs(T10) + std::fabs(T12);
        g1 *= fac2;
        g0 = T22 + std::fabs(T20) + std::fabs(T21);
        g0 *= fac2;
    }

    else {
        IMatrix2D::size_type index = current_lanczos_vector_index - 1;
        double alphaPrev = a[index - 1];
        double alpha = a[index];
        double betaPrev = b[index - 2];
        double beta = b[index - 1];

        double scale = std::max(std::fabs(alphaPrev), std::fabs(alpha));
        scale = std::max(std::fabs(betaPrev), scale);
        scale = std::max(std::fabs(beta), scale);

        alphaPrev /= scale;
        alpha /= scale;
        betaPrev /= scale;
        beta /= scale;

        double fac2 = scale * scale;

        double tmp1 = std::fabs(betaPrev * beta);
        double tmp2 = std::fabs((alphaPrev + alpha) * beta);
        double tmp3 = alpha * alpha;
        double tmp4 = beta * beta;
        g2 += tmp1 * fac2;
        g1 += (tmp4 + tmp2) * fac2;
        g0 = (tmp3 + tmp4 + tmp2 + tmp1) * fac2;
    }

    // take largest eigenvalue approximation
    double g = std::max(g2, g1);
    g = std::max(g0, g);
    g = std::max(g0_prev, g);

    double const eps = std::numeric_limits<double>::epsilon();
    double theta = std::sqrt(A_->cols()) * eps * 0.5 * std::sqrt(g);

    g0_prev = g;

    return theta;
}

} // LinearSolverLibrary_NS
