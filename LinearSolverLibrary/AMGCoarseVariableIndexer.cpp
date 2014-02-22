#include "pch.h"

#include "AMGCoarseVariableIndexer.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGCoarseVariableIndexer::AMGCoarseVariableIndexer(VariableCategorizer const & categorizer) {
    size_type numberOfVariables = categorizer.NumberOfVariables();
    for (size_type i{0}, count{0}; i < numberOfVariables; ++i) {
        if (categorizer.GetType(i) == VariableCategorizer::Type::COARSE)
            map_[i] = count++;
    }
}

AMGCoarseVariableIndexer::size_type
AMGCoarseVariableIndexer::Index(size_type variable_index) const {
    auto it = map_.find(variable_index);
    if (it == std::cend(map_))
        throw std::out_of_range("AMGCoarseVariableIndexer::Index: Variable index out of range");
    return it->second;
}

AMGCoarseVariableIndexer::size_type
AMGCoarseVariableIndexer::NumberOfVariables() const {
    return map_.size();
}
