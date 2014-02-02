#include "pch.h"

#include "AMGDirectInterpolationPolicy.h"
#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableCategorizer.h"
#include "VariableInfluenceAccessor.h"
#include "AMGStandardSplitting.h"

using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGDirectInterpolationPolicy::AMGDirectInterpolationPolicy() {}

bool
AMGDirectInterpolationPolicy::generate(SparseMatrix2D const & m) {
    AMGStandardCoarseningStrengthPolicy strength_policy{m};
    VariableCategorizer variable_categorizer{m.rows()};
    VariableInfluenceAccessor influence_accessor{strength_policy, variable_categorizer};
    AMGStandardSplitting splitting{m, influence_accessor, variable_categorizer};
    splitting.generateSplitting();

    // compute interpolation operator
//    AMGDirectInterpolation

    return true;
}

SparseMatrix2D
AMGDirectInterpolationPolicy::prolongator() const {
    return SparseMatrix2D{5};
}

SparseMatrix2D
AMGDirectInterpolationPolicy::interpolator() const {
    return SparseMatrix2D{ 5 };
}
