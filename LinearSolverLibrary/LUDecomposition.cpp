#include "pch.h"

#include "LUDecomposition.h"



using namespace LinAlg_NS;


namespace LinearSolverLibrary_NS {


namespace {

template<typename T, typename U>
std::unique_ptr<U>
make_unique_ptr(T && arg1, T && arg2) {
    return std::unique_ptr<U>(new U(std::forward<T>(arg1), std::forward<T>(arg2)));
}

}

bool
LUDecomposition::decompose(LinAlg_NS::Matrix2D const & A) const {
    /* LU decomposition of square matrix A using Crout's method
     * with partial pivoting.
     */
    LU_ = std::move(make_unique_ptr<IMatrix2D::size_type, Matrix2D>(A.rows(), A.cols()));

    return true;
}

// LinAlg_NS::Vector
// LUDecomposition::solve(LinAlg_NS::Vector const & rhs) const {
// }

} // LinearSolverLibrary_NS
