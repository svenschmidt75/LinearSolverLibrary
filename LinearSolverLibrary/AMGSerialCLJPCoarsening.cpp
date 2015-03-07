#include "pch.h"

#include "AMGSerialCLJPCoarsening.h"

namespace LinearSolverLibrary_NS{
class IAMGStandardStrengthPolicy;
}

using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


AMGSerialCLJPCoarsening::AMGSerialCLJPCoarsening(SparseMatrix2D const & m, IAMGStandardStrengthPolicy const & strength_policy, IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer & categorizer)
    :
    m_{m},
    strength_policy_{strength_policy},
    variable_influence_accessor_{variable_influence_accessor},
    categorizer_{categorizer} {}

void
AMGSerialCLJPCoarsening::coarsen() {
    initialWeightInitialization();
}

void
AMGSerialCLJPCoarsening::initialWeightInitialization() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for (size_t i = 0; i < m_.cols(); ++i) {
        // get the number of variables that variable i strongly influences
        auto strongly_influenced = strength_policy_.GetDependentOnVariables(i);
        weights_[i] = strongly_influenced->size() + dis(gen);
    }
}

std::vector<AMGSerialCLJPCoarsening::size_type>
AMGSerialCLJPCoarsening::selectIndependentSet() {
    std::vector<size_type> independent_set;
    return independent_set;
}

void
AMGSerialCLJPCoarsening::updateWeights() {
    
}