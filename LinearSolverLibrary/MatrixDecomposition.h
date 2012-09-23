/*
 * Name  : MatrixDecomposition
 * Path  : 
 * Use   : Describes the decomposition of a matrix.
 * Author: Sven Schmidt
 * Date  : 09/23/2012
 */
#pragma once

#include "DeclSpec.h"

#include <cstdint>
#include <vector>


namespace LinAlg_NS {
    class SparseMatrix2D;
}


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS MatrixDecomposition {
    public:
        typedef std::vector<std::uint64_t> ISType;
        typedef ISType::const_iterator elem_const_iterator;

        typedef std::vector<ISType> DataType;
        typedef DataType::const_iterator const_iterator;

    public:
        MatrixDecomposition(LinAlg_NS::SparseMatrix2D const & m, DataType const & is_data);

        const_iterator                    cbegin() const;
        const_iterator                    cend() const;

        LinAlg_NS::SparseMatrix2D const & matrix() const;

    private:
        LinAlg_NS::SparseMatrix2D m_;
        DataType                  is_data_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
