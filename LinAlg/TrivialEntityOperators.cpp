#include "pch.h"

#define ENTITY_OPERATORS
#include "TrivialEntityOperators.h"
#include "NonTrivialEntityOperators.h"
#undef ENTITY_OPERATORS

#include "common/reporting.h"


using namespace LinAlg_NS;


/*************
 * operators *
 *************/

namespace LinAlg_NS {

Vector &
operator*=(Vector & lhs, double scalar) {
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), [scalar](double val){
        return scalar * val;
    });
    return lhs;
}

template<typename VECTOR_EXPR>
Vector &
operator+=(Vector & lhs, VECTOR_EXPR const & rhs) {
#ifdef _DEBUG
    common_NS::reporting::checkConditional(lhs.size() == rhs.size());
#endif
    for (Vector::size_type i = 0; i < lhs.size(); ++i) {
        lhs(i) += rhs(i);
    }
    return lhs;
}

template<typename VECTOR_EXPR>
Vector &
operator-=(Vector & lhs, VECTOR_EXPR const & rhs) {
#ifdef _DEBUG
    common_NS::reporting::checkConditional(lhs.size() == rhs.size());
#endif
    for (Vector::size_type i = 0; i < lhs.size(); ++i) {
        lhs(i) -= rhs(i);
    }
    return lhs;
}

/* Note: Exporting a template function in the .h file makes no sense.
 * Instead, we export the explicitly instantiated function here!
 */
template LINALG_DECL_SYMBOLS Vector & operator+=(Vector & lhs, Vector const & rhs);
template LINALG_DECL_SYMBOLS Vector & operator+=(Vector & lhs, internal::ScalarVectorExpr<Vector, internal::MUL<double>> const & rhs);
template LINALG_DECL_SYMBOLS Vector & operator+=(Vector & lhs, internal::VectorBinaryExpr<Vector, Vector, internal::SUB<double>> const & rhs);

template LINALG_DECL_SYMBOLS Vector & operator-=(Vector & lhs, Vector const & rhs);
template LINALG_DECL_SYMBOLS Vector & operator-=(Vector & lhs, internal::ScalarVectorExpr<Vector, internal::MUL<double>> const & rhs);

} // namespace LinAlg_NS
