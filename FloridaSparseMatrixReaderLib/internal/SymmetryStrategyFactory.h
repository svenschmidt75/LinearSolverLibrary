/*
 * Name  : SymmetryStrategyFactory
 * Path  : ISymmetryStrategy
 * Use   : The Matrix Market format is different depending on whether
           the matrix is symmetric or not.
 * Author: Sven Schmidt
 * Date  : 03/02/2013
 */
#pragma once


#include "ISymmetryStrategy.h"

#include "LinAlg/SparseMatrix2D.h"


namespace EntityReader_NS {

    class SymmetryStrategyFactory {
    public:
        static ISymmetryStrategy::UniquePtr create(std::string const & strategy, LinAlg_NS::SparseMatrix2D & m);
    };

} // EntityReader_NS
