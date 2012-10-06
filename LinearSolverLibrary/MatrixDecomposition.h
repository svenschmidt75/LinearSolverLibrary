/*
 * Name  : MatrixDecomposition
 * Path  : 
 * Use   : Describes the decomposition of a matrix.
 * Author: Sven Schmidt
 * Date  : 09/23/2012
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/SparseMatrix2D.h"

#include <cstdint>
#include <map>
#include <set>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS MatrixDecomposition {
    public:
        typedef std::uint64_t color_t;
        typedef std::map<color_t, std::set<std::uint64_t>> ISO_t;
        typedef ISO_t::const_iterator const_iterator;

    public:
        MatrixDecomposition(ISO_t const & iso_data);

        const_iterator                    cbegin() const;
        const_iterator                    cend() const;

        ISO_t::size_type                  size() const;

    private:
        ISO_t iso_data_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
