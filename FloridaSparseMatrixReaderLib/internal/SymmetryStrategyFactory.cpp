#include "pch.h"


#include "SymmetryStrategyFactory.h"
#include "SymmetricStrategy.h"

#include <stdexcept>


namespace EntityReader_NS {

ISymmetryStrategy::UniquePtr
SymmetryStrategyFactory::create(std::string const & strategy, LinAlg_NS::SparseMatrix2D & m) {
    if (strategy == "symmetric")
        return ISymmetryStrategy::UniquePtr(new SymmetricStrategy(m));

    throw std::runtime_error("SymmetryStrategyFactory::create: Unknown symmetry strategy requested");
}

} // EntityReader_NS
