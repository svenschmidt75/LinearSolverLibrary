/*
 * Name  : AMGCThenFRelaxationPolicy
 * Path  :
 * Use   : Generates a variable decomposition.
 *         The 1st set is all C variables, the next one all F.
 *         Solves via GS.
 * Author: Sven Schmidt
 * Date  : 03/30/2014
 */
#pragma once


// forward declarations
namespace LinAlg_NS {

    class Vector;
    class SparseMatrix2D;
}

namespace LinearSolverLibrary_NS {

    class VariableCategorizer;
}


namespace LinearSolverLibrary_NS {

    class AMGCThenFRelaxationPolicy {
    public:
        template<typename AMGInterpolationPolicy>
        MatrixDecomposition Decompose(AMGInterpolationPolicy const & interpolation_policy) const {
            VariableCategorizer const & variable_categorizer = interpolation_policy.GetVariableCategorizer();
            MatrixDecomposition::ISO_t decomposition;
            auto count = variable_categorizer.NumberOfVariables();
            for (auto index = 0; index < count; ++index) {
                int priority;
                if (variable_categorizer.GetType(index) == VariableCategorizer::Type::COARSE)
                    priority = 0;
                else if (variable_categorizer.GetType(index) == VariableCategorizer::Type::FINE)
                    priority = 1;
                else {
                    throw std::runtime_error("AMGCThenFRelaxationPolicy::Decompose: Unrecognized variable type");
                }
                decomposition[priority].insert(index);
            }
            return MatrixDecomposition{decomposition};
        }

        LinAlg_NS::Vector Solve(MatrixDecomposition const & decompsition, LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & b, LinAlg_NS::Vector const & x_initial) const {
            return LinAlg_NS::Vector{0};
        }
    };

} // LinearSolverLibrary_NS
