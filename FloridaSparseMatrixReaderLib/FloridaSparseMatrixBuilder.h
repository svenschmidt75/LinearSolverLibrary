/*
 * Name  : SparseMatrixBuilder
 * Path  : 
 * Use   : Builds a sparse matrix
 * Author: Sven Schmidt
 * Date  : 08/12/2012
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrixBuilder.h"

#include "LinAlg/IMatrix2D.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


// forward declarations
namespace LinAlg_NS {
    class SparseMatrix2D;
}

namespace EntityReader_NS {

    class FLORIDASPARSEMATRIXREADERLIB_DECL_SYMBOLS FloridaSparseMatrixBuilder : public IMatrixBuilder<LinAlg_NS::SparseMatrix2D> {
    public:
        void     init(LinAlg_NS::IMatrix2D::size_type dim) const;
        void     insertMatrixElement(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) const;
        void     finalize() const;
        result_t result() const;

    private:
        mutable result_t                        data_;
        mutable LinAlg_NS::IMatrix2D::size_type dim_;
    };

} // namespace EntityReader_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
