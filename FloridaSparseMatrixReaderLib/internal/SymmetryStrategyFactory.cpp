#include "pch.h"


#include "SymmetryStrategyFactory.h"
#include "SymmetricStrategy.h"
#include "UnsymmetricStrategy.h"


namespace EntityReader_NS {

ISymmetryStrategy::UniquePtr
SymmetryStrategyFactory::create(std::string const & strategy, LinAlg_NS::SparseMatrix2D & m) {
    if (strategy == "symmetric")
        return ISymmetryStrategy::UniquePtr(new SymmetricStrategy(m));

    if (strategy == "general")
        return ISymmetryStrategy::UniquePtr(new UnsymmetricStrategy(m));

    throw std::runtime_error("SymmetryStrategyFactory::create: Unknown symmetry strategy requested");
}

} // EntityReader_NS
