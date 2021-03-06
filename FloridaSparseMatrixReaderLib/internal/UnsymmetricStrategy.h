/*
 * Name  : UnsymmetricStrategy
 * Path  : ISymmetryStrategy
 * Use   : Unsymmetrical matrix in MatrixMarket format.
 * Author: Sven Schmidt
 * Date  : 03/03/2013
 */
#pragma once


#include "ISymmetryStrategy.h"


namespace LinAlg_NS {
    class SparseMatrix2D;
}


namespace EntityReader_NS {


    class UnsymmetricStrategy : public ISymmetryStrategy {
    public:
        UnsymmetricStrategy(LinAlg_NS::SparseMatrix2D & m);
        UnsymmetricStrategy(UnsymmetricStrategy const &) = delete;
        UnsymmetricStrategy & operator=(UnsymmetricStrategy const &) = delete;

        // FROM ISymmetryStrategy
        void insert(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) final;
        bool check() const final;

    private:
        LinAlg_NS::SparseMatrix2D & m_;
    };

} // EntityReader_NS
