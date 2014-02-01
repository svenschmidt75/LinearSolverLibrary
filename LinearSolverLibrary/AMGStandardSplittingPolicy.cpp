#include "pch.h"

#include "AMGStandardSplittingPolicy.h"
#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableCategorizer.h"
#include "VariableInfluenceAccessor.h"
#include "AMGStandardSplitting.h"

using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGStandardSplittingPolicy::AMGStandardSplittingPolicy(LinAlg_NS::SparseMatrix2D const & m)
    :
    m_{m} {
    // compute C/F splitting
    strength_policy_ = std::make_unique<AMGStandardCoarseningStrengthPolicy>(m);
    variable_categorizer_ = std::make_unique<VariableCategorizer>(m.rows());
    variable_influence_accessor_ = std::make_unique<VariableInfluenceAccessor>(*strength_policy_, *variable_categorizer_);
    splitting_ = std::make_unique<AMGStandardSplitting>(m, *variable_influence_accessor_, *variable_categorizer_);
}
