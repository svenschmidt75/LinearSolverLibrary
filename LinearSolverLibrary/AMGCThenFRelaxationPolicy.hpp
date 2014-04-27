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


#include "Monitor.h"


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

        static std::tuple<bool, LinAlg_NS::Vector> Solve(LinAlg_NS::SparseMatrix2D const & m ,LinAlg_NS::Vector const & x_initial, LinAlg_NS::Vector const & b, MatrixDecomposition const & decomposition, short max_iterations, Monitor & monitor) {
            // SparseGSMultiColor is used as the inner iteration scheme.
            // In there, Monitor will be modified, but since we want
            // to interpret these values based on the outer, AMG, iteration,
            // we have to save its state and restore it.
            Monitor backup = monitor;
            std::tuple<bool, LinAlg_NS::Vector> result;

            try {
                monitor.nmax_iterations = max_iterations;
                result = SparseLinearSolverLibrary::SparseGSMultiColor(m, x_initial, b, decomposition, monitor);
            } catch (...) {}

            monitor = backup;
            return result;
        }
    };

} // LinearSolverLibrary_NS
