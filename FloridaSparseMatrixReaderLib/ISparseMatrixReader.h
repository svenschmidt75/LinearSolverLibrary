/*
 * Name  : ISparseMatrixReader
 * Path  : 
 * Use   : Base class for reading sparse matrices. Makes sense, because
           the client need not know how the sparse matrix data is stored
           internally.
 * Author: Sven Schmidt
 * Date  : 09/08/2012
 */
#pragma once

#include <memory>


namespace LinAlg_NS {
    class SparseMatrix2D;
}

namespace EntityReader_NS {

    class ISparseMatrixReader {
    public:
        typedef std::unique_ptr<ISparseMatrixReader> Ptr;

    public:
        virtual ~ISparseMatrixReader() {}

        virtual bool                            read() const = 0;
        virtual LinAlg_NS::SparseMatrix2D const get() const = 0;
    };

} // namespace EntityReader_NS
