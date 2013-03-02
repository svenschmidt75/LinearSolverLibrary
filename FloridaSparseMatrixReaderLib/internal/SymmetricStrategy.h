/*
 * Name  : SymmetricStrategy
 * Path  : ISymmetryStrategy
 * Use   : Makes sure the matrix is symmetric when constructed.
 * Author: Sven Schmidt
 * Date  : 03/02/2013
 */
#pragma once


#include "ISymmetryStrategy.h"

#include "LinAlg/IMatrix2D.h"

#include <boost/noncopyable.hpp>


namespace LinAlg_NS {
    class SparseMatrix2D;
}


namespace EntityReader_NS {


    class SymmetricStrategy : public ISymmetryStrategy, boost::noncopyable {
    public:
        SymmetricStrategy(LinAlg_NS::SparseMatrix2D & m);

        // FROM ISymmetryStrategy
        void insert(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) final;

    private:
        LinAlg_NS::SparseMatrix2D & m_;
    };

} // EntityReader_NS
