/*
 * Name  : SparseMatrixBinaryReader
 * Path  : 
 * Use   : Base class for reading sparse matrices. Makes sense, because
           the client need not know how the sparse matrix data is stored
           internally.
 * Author: Sven Schmidt
 * Date  : 09/08/2012
 */
#pragma once

#include "ISparseMatrixReader.h"

#include "LinAlg/SparseMatrix2D.h"


namespace EntityReader_NS {

    class SparseMatrix2D;

    class SparseMatrixBinaryReader : public ISparseMatrixReader {
    public:
        bool                      read() const override;
        LinAlg_NS::SparseMatrix2D get() const override;

    private:
        LinAlg_NS::SparseMatrix2D m_;
    };

} // namespace EntityReader_NS
