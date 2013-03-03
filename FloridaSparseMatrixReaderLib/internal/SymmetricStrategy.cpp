#include "pch.h"


#include "SymmetricStrategy.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/helper.h"


namespace EntityReader_NS {

SymmetricStrategy::SymmetricStrategy(LinAlg_NS::SparseMatrix2D & m) : m_(m) {}

void
SymmetricStrategy::insert(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) {
    m_(row, col) = value;

    // ensure matrix is symmetric
    if (row != col)
        m_(col, row) = value;
}

bool
SymmetricStrategy::check() const {
    // check that the matrix is indeed symmetric
    return LinAlg_NS::helper::isSymmsteric(m_);
}

} // EntityReader_NS