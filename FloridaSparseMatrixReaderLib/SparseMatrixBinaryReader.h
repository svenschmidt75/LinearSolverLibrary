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


namespace EntityReader_NS {

    class SparseMatrixBinaryReader : public ISparseMatrixReader {
    public:
        SparseMatrixBinaryReader(std::string const & filename);

        // FROM ISparseMatrixReader
        bool                            read() const override;
        LinAlg_NS::SparseMatrix2D const get() const override;

    private:
        std::string                       filename_;
        mutable LinAlg_NS::SparseMatrix2D data_;
    };

} // namespace EntityReader_NS
