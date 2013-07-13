/*
 * Name  : LanczosPRO
 * Path  : 
 * Use   : Lanczos algorithm with partial reorthogonalization
 * Author: Sven Schmidt
 * Date  : 07/04/2013
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/entities.h"

#include <functional>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS LanczosPRO final : boost::noncopyable {
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

    private:
        void monitorOrthogonality() const;
        bool checkForReorthogonalization(LinAlg_NS::IMatrix2D::size_type index) const;
        void reorthogonalizeLanczosVector(LinAlg_NS::IMatrix2D::size_type index) const;
        void findLanczosVectorsToReorthogonalizeAgainst(LinAlg_NS::IMatrix2D::size_type index) const;
        double computeLanczosNorm() const;
        void initializeOmega() const;
        void rotateOmega() const;

    private:
        // orthogonalized via Lanczos
        mutable std::vector<LinAlg_NS::Vector> q;
        mutable LinAlg_NS::IMatrix2D::size_type current_lanczos_vector_index;

        // alphas and bets in T_i
        mutable std::vector<double> a;
        mutable std::vector<double> b;

        // for monitoring the orthogonality
        mutable std::vector<double> w1;
        mutable std::vector<double> w2;
        mutable std::vector<double> w3;

        mutable std::function<decltype(w1) & ()> omega1;
        mutable std::function<decltype(w1) & ()> omega2;
        mutable std::function<decltype(w1) & ()> omega3;

        mutable std::vector<LinAlg_NS::IMatrix2D::size_type> indices;

        mutable LinAlg_NS::SparseMatrix2D A_;
        mutable bool force_reorthogonalization;

        mutable double g1;
        mutable double g2;
        mutable double g0_prev;

        mutable int numer_of_reorthogonalizations;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
