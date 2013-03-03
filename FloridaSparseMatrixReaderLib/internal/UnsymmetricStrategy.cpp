#include "pch.h"


#include "UnsymmetricStrategy.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/helper.h"


namespace EntityReader_NS {

UnsymmetricStrategy::UnsymmetricStrategy(LinAlg_NS::SparseMatrix2D & m) : m_(m) {}

void
UnsymmetricStrategy::insert(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) {
    m_(row, col) = value;
}

bool
UnsymmetricStrategy::check() const {
    // nothing to check
    return true;
}

} // EntityReader_NS
