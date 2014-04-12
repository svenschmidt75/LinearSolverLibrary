/*
 * Name  : MatrixDecomposition
 * Path  : 
 * Use   : Describes the decomposition of a matrix.
 * Author: Sven Schmidt
 * Date  : 09/23/2012
 */
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS MatrixDecomposition {
    public:
        typedef std::uint64_t color_t;
        typedef std::map<color_t, std::set<std::uint64_t>> ISO_t;
        typedef ISO_t::const_iterator const_iterator;
        typedef ISO_t::size_type size_type;

    public:
        MatrixDecomposition();
        MatrixDecomposition(ISO_t const & iso_data);
        MatrixDecomposition(MatrixDecomposition && in);

        // should be cbegin/cend, but range based for looks
        // for begin/end
        const_iterator   begin() const;
        const_iterator   end() const;

        size_type        size() const;

    private:
        ISO_t iso_data_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
