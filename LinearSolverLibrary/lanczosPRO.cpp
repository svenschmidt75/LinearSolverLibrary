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
    }
}

void
LanczosPRO::init(LinAlg_NS::SparseMatrix2D const & A, Vector const & q0) const {
    A_ = A;
    IMatrix2D::size_type dim = A_.cols();

    /* Use
     * 1. reserve instead of resize, so not all the vectors have to be default-created
     *    immediately
     * 2. use push_back
     */
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
    BOOST_ASSERT_MSG(current_lanczos_vector_index < A_.cols(), "Lanczos::computeNextLanczosVector: Insufficient space");
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

    if (force_reorthogonalization) {
        reorthogonalizeLanczosVector(current_lanczos_vector_index - 1);
        force_reorthogonalization = false;
    }

    double theta = computeLanczosNorm();

    auto i = current_lanczos_vector_index - 1;
    if (i > 1) {
        for (IMatrix2D::size_type j = 0; j < i - 1; ++j) {
            double beta_ip1 = b[i];

            double beta_jp1 = b[j + 1];
            double omega_i_jp1 = i - 1 == j + 1 ? 1.0 : omega2()[j];
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
            double omega_j_im1 = j  + 1 == i - 1 ? 1.0 : omega1()[j];
            double term4 = beta_i * omega_j_im1 / beta_ip1;

            double term5 = theta / beta_ip1;

            double sum = (beta_jp1 * omega_i_jp1 + aa * omega_i_j + beta_j * omega_i_jm1 - beta_i * omega_j_im1 + theta) / beta_ip1;
            sum = term1 + term2 + term3 - term4 + term5;
            omega3()[j] = sum;

            double angle = VectorMath::dotProduct(q[i], q[j]);

//            std::cout << std::endl << "(" << i << "," << j << "): is/true: " << sum << ":" << angle;

            angle = angle;
        }
    }
    omega3()[i - 1] = std::sqrt(A_.cols()) * 0.5 * eps;

    bool reorthogonalize = checkForReorthogonalization(i);
    if (reorthogonalize) {
        numer_of_reorthogonalizations++;
        std::cout << std::endl << "reorthogonalization " << numer_of_reorthogonalizations;

        printLanczosVectorsOrthogonal(q, current_lanczos_vector_index);

        findLanczosVectorsToReorthogonalizeAgainst(current_lanczos_vector_index - 1);
        reorthogonalizeLanczosVector(current_lanczos_vector_index - 1);

        force_reorthogonalization = true;
    }

    // TODO: Use indices instead of copying
    /* Use a lambda function for accessing, i.e.
     * 
     * std::function<> last() {
     *   return [this, &w3]() -> std::vector<double> & {
     *     return w1;
     *   }
     *   
     *   Then: last()[index] = eps; or so
     * }
     * 
     * */
    rotateOmega();
//     w1 = w2;
//     w2 = w3;
}

void
LanczosPRO::rotateOmega() const {
    decltype(omega1) tmp = omega1;

    omega1 = omega2;
    omega2 = omega3;
    omega3 = tmp;
}

bool
LanczosPRO::checkForReorthogonalization(IMatrix2D::size_type index) const {
    double const eps = std::numeric_limits<double>::epsilon();
    double const eps2 = std::sqrt(eps);
    double max_value = std::numeric_limits<double>::min();
    for (IMatrix2D::size_type j = 0; j < index; ++j) {
        max_value = std::max(max_value, std::fabs(omega3()[j]));
        if (max_value > eps2)
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
    Vector & q_prev = q[index];
    printLanczosVectorsOrthogonal(q, index + 1);
    for (IMatrix2D::size_type i = 0; i < indices.size(); ++i) {
        decltype(i) lanczos_index = indices[i];
        if (lanczos_index) {
            double proj = VectorMath::dotProduct(q_prev, q[i]);
            q_prev -= proj * q[i];
            double const eps = std::numeric_limits<double>::epsilon();

            // TODO SS: Is omega3() reinit needed? Its thrown away the next iteration
            omega3()[i] = std::sqrt(A_.cols()) * 0.5 * eps;

            omega2()[i] = std::sqrt(A_.cols()) * 0.5 * eps;
        }
    }
    double norm = VectorMath::norm(q_prev);
    q_prev = q_prev * (1.0 / norm);
    printLanczosVectorsOrthogonal(q, index + 1);
}

double
LanczosPRO::computeLanczosNorm() const {
    /* Compute norm of matrix A via the symmetric matrix T.
     * Uses Gershgorin's circle theorem to estimate the largest
     * eigenvalue of T.
     */
    double g0;
    if (current_lanczos_vector_index == 2) {
        g0_prev = 0;
        g2 = 0;

        double alpha1 = a[1];
        double beta2 = b[1];
        double T00 = alpha1 * alpha1 + beta2 * beta2;
        double T01 = alpha1 * beta2;
        double T10 = T01;
        double T11 = beta2 * beta2;

        // 1st and 2nd row of T_{1}
        g1 = T00 + std::fabs(T01);
        g0 = T11 + std::fabs(T10);
    }

    // this uses Gershgorin's circle theorem
    else if (current_lanczos_vector_index == 3) {
        double alpha1 = a[1];
        double alpha2 = a[2];
        double beta2 = b[1];
        double beta3 = b[2];
        double T00 = alpha1 * alpha1 + beta2 * beta2;
        double T01 = (alpha1 + alpha2) * beta2;
        double T02 = beta2 * beta3;
        double T10 = T01;
        double T11 = beta2 * beta2 + alpha2 * alpha2 + beta3 * beta3;
        double T12 = alpha2 * beta3;
        double T20 = T02;
        double T21 = T12;
        double T22 = beta3 * beta3;

        g2 = T00 + std::fabs(T01) + std::fabs(T02);
        g1 = T11 + std::fabs(T10) + std::fabs(T12);
        g0 = T22 + std::fabs(T20) + std::fabs(T21);
    }

    else {
        IMatrix2D::size_type index = current_lanczos_vector_index - 1;
        double alphaPrev = a[index - 1];
        double alpha = a[index];
        double betaPrev = b[index - 1];
        double beta = b[index];

        double tmp1 = std::fabs(betaPrev * beta);
        double tmp2 = std::fabs((alphaPrev + alpha) * beta);
        g2 += tmp1;
        g1 += (beta * beta + tmp2);
        g0 = beta * beta + alpha * alpha + tmp2 + tmp1;
    }

    // take largest eigenvalue approximation
    double g = std::max(g2, g1);
    g = std::max(g0, g);
    g = std::max(g0_prev, g);

    double const eps = std::numeric_limits<double>::epsilon();
    double theta = std::sqrt(A_.cols()) * eps * 0.5 * std::sqrt(g);

    g0_prev = g;

    return theta;
}

} // LinearSolverLibrary_NS
