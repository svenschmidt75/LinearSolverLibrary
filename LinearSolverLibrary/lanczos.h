/*
 * Name  : Lanczos
 * Path  : 
 * Use   : Lanczos algorithm
 * Author: Sven Schmidt
 * Date  : 07/04/2013
 * 
 * Iterative Methods For solving Linear Systems, Anne Greenbaum, SIAM, 1997
 * p. 42
 */
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)



namespace LinAlg_NS {

    namespace internal {

        template<typename VECTOR_EXPR, typename BINOP>
        class ScalarVectorBinaryExpr;

    } // namespace internal

} // namespace LinAlg_NS


namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS Lanczos final : boost::noncopyable {
    public:
        void init(LinAlg_NS::SparseMatrix2D const & A, LinAlg_NS::Vector const & q0) const;

        template<typename EXPR, typename BINOP>
        void init(LinAlg_NS::SparseMatrix2D const & A, LinAlg_NS::internal::ScalarVectorBinaryExpr<EXPR, BINOP> const & q0) const {
            // really need inheriting constructors here :-(
            LinAlg_NS::Vector q;
            q = q0;
            init(A, q);
        }

        void computeNextLanczosVector() const;
        double getCurrentAlpha() const;
        double getCurrentBeta() const;
        LinAlg_NS::Vector const & getPreviousLanczosVector() const;
        double getPreviousAlpha() const;
        double getPreviousBeta() const;
        void extendCapacity(LinAlg_NS::IMatrix2D::size_type new_size) const;

    private:
        // orthogonalized via Lanczos
        mutable std::vector<LinAlg_NS::Vector> q;
        mutable LinAlg_NS::IMatrix2D::size_type current_lanczos_vector_index;

        // alphas and bets in T_i
        mutable std::vector<double> a;
        mutable std::vector<double> b;

        mutable LinAlg_NS::SparseMatrix2D const * A_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
