/*
 * Name  : SymmetricStrategy
 * Path  : ISymmetryStrategy
 * Use   : Makes sure the matrix is symmetric when constructed.
 * Author: Sven Schmidt
 * Date  : 03/02/2013
 */
#pragma once


#include "ISymmetryStrategy.h"


namespace LinAlg_NS {
    class SparseMatrix2D;
}


namespace EntityReader_NS {


    class SymmetricStrategy : public ISymmetryStrategy {
    public:
        SymmetricStrategy(LinAlg_NS::SparseMatrix2D & m);
        SymmetricStrategy(SymmetricStrategy const &) = delete;
        SymmetricStrategy & operator=(SymmetricStrategy const &) = delete;

        // FROM ISymmetryStrategy
        void insert(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) final;
        bool check() const final;

    private:
        LinAlg_NS::SparseMatrix2D & m_;
    };

} // EntityReader_NS
