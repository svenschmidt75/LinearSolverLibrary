#include "pch.h"

#include "VariableCategorizer.h"

#include "common/reporting.h"


using namespace LinearSolverLibrary_NS;


VariableCategorizer::VariableCategorizer(size_type numberOfVariables) {
    data_.resize(numberOfVariables);
    initializeVariablesToUndefined();
}

void
VariableCategorizer::initializeVariablesToUndefined() {
    std::fill(std::begin(data_), std::end(data_), Type::UNDEFINED);
}

VariableCategorizer::Type
LinearSolverLibrary_NS::VariableCategorizer::GetType(size_type variable) const {
    common_NS::reporting::checkUppderBound(variable, data_.size());
    return data_[variable];
}

void
VariableCategorizer::SetType(size_type variable, Type type) {
    common_NS::reporting::checkUppderBound(variable, data_.size());
    data_[variable] = type;
}
