/*
* Name  : AMGStandardCoarseningStrengthPolicy
* Path  :
* Use   : Determines that strong connections of a variable on others, i.e.
          S_{i} and S_{i}^{T}, see Trottenberg, page 473, 474
* Author: Sven Schmidt
* Date  : 10/12/2013
*/
#pragma once

#include "DeclSpec.h"

#include "LinAlg/SparseMatrix2D.h"

#include <boost/noncopyable.hpp>

#include <memory>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardCoarseningStrengthPolicy : boost::noncopyable {
    public:
        AMGStandardCoarseningStrengthPolicy(LinAlg_NS::SparseMatrix2D const & m);

        bool VariableDependsOn(LinAlg_NS::IMatrix2D::size_type source, LinAlg_NS::IMatrix2D::size_type dest) const;

    private:
        void computeConnections();
        void computeConnectionsForVariable(LinAlg_NS::IMatrix2D::size_type i, double max_element);

    private:
        std::unique_ptr<LinAlg_NS::SparseMatrix2D> strength_matrix_;
        LinAlg_NS::SparseMatrix2D const &          m_;
        double const                               eps_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
