/*
 * Name  : FloridaSparseMatrixReader
 * Path  : 
 * Use   : Reads a matrix in the format specified by the
           "The University of Florida Sparse Matrix Collection"
 * Author: Sven Schmidt
 * Date  : 08/12/2012
 */
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinAlg_NS {
    class SparseMatrix2D;
}


namespace EntityReader_NS {

    template<typename T>
    class IMatrixBuilder;

    class FLORIDASPARSEMATRIXREADERLIB_DECL_SYMBOLS FloridaSparseMatrixReader {
    public:
        FloridaSparseMatrixReader(std::string const & filename, IMatrixBuilder<LinAlg_NS::SparseMatrix2D> & builder);
        FloridaSparseMatrixReader(FloridaSparseMatrixReader const &) = delete;
        FloridaSparseMatrixReader & operator=(FloridaSparseMatrixReader const &) = delete;

        bool read() const;

    private:
        void checkHeader(std::vector<std::string> const & str_array, std::uint64_t line_number) const;

    private:
        std::string                                 filename_;
        IMatrixBuilder<LinAlg_NS::SparseMatrix2D> & builder_;
    };

} // namespace EntityReader_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
