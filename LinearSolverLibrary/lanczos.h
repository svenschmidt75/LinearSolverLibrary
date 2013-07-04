/*
 * Name  : Lanczos
 * Path  : 
 * Use   : Lanczos algorithm
 * Author: Sven Schmidt
 * Date  : 07/04/2013
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/entities.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS Lanczos final : boost::noncopyable {
    public:
        Lanczos(LinAlg_NS::SparseMatrix2D const & A, LinAlg_NS::Vector const & q0);

    private:
        LinAlg_NS::Vector const & getLastLanczosVector() const;
        double getLastAlpha() const;
        double getLastBeta() const;
        void computeNextLanczosVector() const;

    private:
        // orthogonalized via Lanczos
        mutable std::vector<LinAlg_NS::Vector> q;
        mutable LinAlg_NS::IMatrix2D::size_type current_lanczos_vector_index;

        // alphas and bets in T_i
        mutable std::vector<double> a;
        mutable std::vector<double> b;

        LinAlg_NS::SparseMatrix2D const & A_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
