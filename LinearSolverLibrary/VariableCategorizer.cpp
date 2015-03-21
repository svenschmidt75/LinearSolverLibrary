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

VariableCategorizer::size_type
VariableCategorizer::NumberOfVariables() const {
    return data_.size();
}

//// Fill with whitespace, right align
//ss << format("#1.0 %4i %4i %4in") % year % month % day;
//// Output: #1.0 2011    3   11

void
VariableCategorizer::print() const {
    std::cout << std::endl;
    int row_size = static_cast<int>(std::sqrt(data_.size()));
    if (row_size * row_size != data_.size()) {
        int const chunk_size = 4;
        // data is not square
        auto chunks = data_.size() / chunk_size;
        for (size_t i = 0; i < chunks; ++i) {
            for (size_t col = 0; col < chunk_size; ++col) {
                auto index = i * chunk_size + col;
                std::cout << boost::format(" %2i: %s --  ") % index % static_cast<char>(data_[index]);
            }
            std::cout << std::endl;
        }
        chunks = data_.size() % chunk_size;
        auto index = data_.size() - chunks;
        for (size_t col = 0; col < chunks; ++col)
            std::cout << boost::format(" %2i: %s --  ") % (index + col) % static_cast<char>(data_[index + col]);
    }
    else {
        for (size_t row = 0; row < row_size; ++row) {
            std::cout << std::endl;
            for (size_t col = 0; col < row_size; ++col) {
                auto index = row * row_size + col;
                std::cout << std::setw(2) << static_cast<char>(data_[index]);
            }
        }
    }
    std::cout << std::endl;
}
