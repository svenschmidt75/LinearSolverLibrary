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

void
VariableCategorizer::print() const {
    int row_size = static_cast<int>(std::sqrt(data_.size()));
    std::cout << std::endl;
    for (size_t row = 0; row < row_size; ++row) {
        std::cout << std::endl;
        for (size_t col = 0; col < row_size; ++col) {
            auto index = row * row_size + col;
            std::cout << std::setw(2) << static_cast<char>(data_[index]);
        }
    }
    std::cout << std::endl;
}
