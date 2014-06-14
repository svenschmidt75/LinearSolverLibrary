/*
 * Name  : AMGStandardStrengthPolicy
 * Path  : IAMGStandardStrengthPolicy
 * Use   : Determines that strong connections of a variable on others, i.e.
           S_{i} and S_{i}^{T}, see Trottenberg, page 473, 474.
           The strength variable i is connected to variables j is measured
           based on the smallest negative value a_{ij} * eps.
 * Author: Sven Schmidt
 * Date  : 10/12/2013
 */
#pragma once

#include "DeclSpec.h"

#include "IAMGStandardStrengthPolicy.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IVariableSet;
}


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardStrengthPolicy : public IAMGStandardStrengthPolicy {
    public:
        AMGStandardStrengthPolicy(LinAlg_NS::SparseMatrix2D const & m);
        AMGStandardStrengthPolicy(AMGStandardStrengthPolicy const &) = delete;
        AMGStandardStrengthPolicy & operator=(AMGStandardStrengthPolicy const &) = delete;

        std::shared_ptr<IVariableSet> GetInfluencedByVariables(LinAlg_NS::IMatrix2D::size_type variable) const override;
        std::shared_ptr<IVariableSet> GetDependentOnVariables(LinAlg_NS::IMatrix2D::size_type variable) const override;

    private:
        void computeConnections();
        void computeConnectionsForVariable(LinAlg_NS::IMatrix2D::size_type i, double max_element);

    private:
        /* Each row contains the strong dependencies, i.e.
           Si_(row, col) != 0 means that variable 'row' is strongly negatively coupled to variable 'col'.
           see Trottenberg, p. 473.
        */
        std::unique_ptr<LinAlg_NS::SparseMatrix2D> Si_;

        /* Each row in Sit_ contains which other variables are strongly influenced, i.e.
           Sit_(row, col) != 0 means that variable 'row' strongly influences variable 'col'.
           see Trottenberg, p. 474.
        */
        std::unique_ptr<LinAlg_NS::SparseMatrix2D> Sit_;
        LinAlg_NS::SparseMatrix2D const &          m_;
        double const                               eps_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
