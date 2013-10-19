#include "pch.h"

#include "VariableCategorizer.h"


using namespace LinearSolverLibrary_NS;


VariableCategorizer::VariableCategorizer(size_type numberOfVariables) {
    data_.resize(numberOfVariables);
    initializeVariablesToUndefined();
}

void
VariableCategorizer::initializeVariablesToUndefined() {
    std::fill(std::begin(data_), std::end(data_), Type::UNDEFINED);
}
