#include "pch.h"

#include "AMGStandardCoarsening.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGStandardCoarsening::AMGStandardCoarsening(LinAlg_NS::SparseMatrix2D const & m, VariableCategorizer & categorizer)
    :
    m_(m), categorizer_(categorizer), strength_policy_(m_) {}
