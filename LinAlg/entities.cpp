#include "pch.h"

#include "entities.h"

#include "common/reporting.h"

using namespace LinAlg_NS;



/*************
 * operators *
 *************/

namespace LinAlg_NS {

Vector
operator-(Vector const & lhs, Vector const & rhs) {
    bool cond = lhs.size() == rhs.size();

    BOOST_ASSERT_MSG(cond, "operator-(Vector, Vector): Size mismatch");
    if (!cond) {
        boost::format format = boost::format("operator-(Vector, Vector): Size mismatch\n");
        common_NS::reporting::error(format.str());
        throw std::runtime_error(format.str());
    }

    Vector tmp(lhs.size());
    std::transform(lhs.cbegin(), lhs.cend(), rhs.cbegin(), tmp.begin(), [](double lhs, double rhs){
        return lhs - rhs;
    });
    return tmp;
}

template<typename MATRIX_EXPR, typename VECTOR_EXPR>
Vector
operator-(Vector const & lhs, internal::MatrixVectorExpr<MATRIX_EXPR, VECTOR_EXPR> const & rhs) {
    bool cond = lhs.size() == rhs.size();

    BOOST_ASSERT_MSG(cond, "operator-(Vector, Vector): Size mismatch");
    if (!cond) {
        boost::format format = boost::format("operator-(Vector, Vector): Size mismatch\n");
        common_NS::reporting::error(format.str());
        throw std::runtime_error(format.str());
    }

    Vector tmp(lhs.size());

    for (Vector::size_type i = 0; i < lhs.size(); ++i) {
        tmp(i) = lhs(i) - rhs(i);
    }

    return tmp;
}

Vector
operator*(double value, Vector const & v) {
    Vector tmp(v);

    std::for_each(tmp.begin(), tmp.end(), [value](double val) {
        return val * value;
    });

    return tmp;
}

Vector &
operator+=(Vector & lhs, Vector const & rhs) {
    std::transform(lhs.cbegin(), lhs.cend(), rhs.cbegin(), lhs.begin(), [](double lhs, double rhs){
        return lhs + rhs;
    });

    return lhs;
}

template<typename VECTOR_EXPR, typename BINOP>
Vector &
operator+=(Vector & lhs, internal::ScalarVectorBinaryExpr<VECTOR_EXPR, BINOP> const & rhs) {
    for (Vector::size_type i = 0; i < lhs.size(); ++i) {
        lhs(i) += rhs(i);
    }

    return lhs;
}

// explicit instantiations


/* Note: Exporting a template function in the .h file makes no sense.
 * Instead, we export the explicitly instantiated function here!
 */
template LINALG_DECL_SYMBOLS Vector operator-<SparseMatrix2D, Vector>(Vector const & lhs, internal::MatrixVectorExpr<SparseMatrix2D, Vector> const & rhs);

} // namespace LinAlg_NS
