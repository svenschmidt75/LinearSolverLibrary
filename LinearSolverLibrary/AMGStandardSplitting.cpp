#include "pch.h"

#include "AMGStandardSplitting.h"


using namespace LinearSolverLibrary_NS;


AMGStandardSplitting::AMGStandardSplitting(IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer const & categorizer)
    :
    variable_influence_accessor_(variable_influence_accessor),
    categorizer_(categorizer) {}
