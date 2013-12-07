/*
 * Name  : SparseMatrix2D
 * Path  : IMatrix2D
 * Use   : Implements a sparse 2D matrix representation.
 *         The matrix elements are first inserted and once finished,
 *         the internal matrix representation is converted into the
 *         compressed storage format.
 *         Note 1: We are storing matrices in row-major format, hence here,
 *               we implement the compressed row format.
 *         Note 2: Empty rows are NOT supported!
 * Author: Sven Schmidt
 * Date  : 12/26/2011
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"

#include <boost/cstdint.hpp>

#include <map>
#include <vector>


namespace boost {
    namespace serialization {
        class access;
    }

    namespace archive {
        class text_oarchive;
        class text_iarchive;
        class xml_oarchive;
        class xml_iarchive;
        class binary_oarchive;
        class binary_iarchive;
    }
}

namespace LinearSolverLibrary_NS {
    class SparseLinearSolverUtil;
    class SparseLinearSolverLibrary;
}

/* This class is used as a friend here, and is declared in UnitTest in the global
 * namespace. Hence, we have to forward-declare it here, otherwise the compiler
 * complains about an ambiguity as it looks in namespace LinAlg_NS.
 */
class FloridaSparseMatrixReaderTest;


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {

    class Vector;

    template<typename T>
    class ConstColumnIterator;

    template<typename T>
    class ConstRowIterator;

    class LINALG_DECL_SYMBOLS SparseMatrix2D : public IMatrix2D {

        // support for boost::serialize
        friend class boost::serialization::access;
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::text_oarchive & ar, SparseMatrix2D & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::text_iarchive & ar, SparseMatrix2D & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::xml_oarchive & ar, SparseMatrix2D & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::xml_iarchive & ar, SparseMatrix2D & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::binary_oarchive & ar, SparseMatrix2D & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::binary_iarchive & ar, SparseMatrix2D & m, const unsigned int version);
        template<typename AR>
        friend void serialize_helper(AR & ar, SparseMatrix2D & m, const unsigned int /*version*/);

        friend class LinearSolverLibrary_NS::SparseLinearSolverUtil;
        friend class LinearSolverLibrary_NS::SparseLinearSolverLibrary;
        friend class helper;
        friend class FloridaSparseMatrixReaderTest;
        friend class ConstColumnIterator<SparseMatrix2D>;
        friend class ConstRowIterator<SparseMatrix2D>;

    public:
        SparseMatrix2D();
        SparseMatrix2D(size_type dimension);
        SparseMatrix2D(size_type rows, size_type columns);
        SparseMatrix2D(SparseMatrix2D const & in);
        SparseMatrix2D & operator=(SparseMatrix2D const & in);

        // enable move semantics
        SparseMatrix2D(SparseMatrix2D && in);
        SparseMatrix2D & operator=(SparseMatrix2D && in);

        // FROM IMatrix2D
        size_type rows() const;
        size_type cols() const;
        double    operator()(size_type row, size_type column) const;
        double &  operator()(size_type row, size_type column);

        // Local methods
        void solve(Vector const & b, Vector & x) const;
        void finalize() const;
        void print() const;

    private:
        typedef std::map<size_type, double> Col_t;
        typedef std::map<size_type, Col_t> Row_t;

    private:
        // to provide exception-safe copy-assignment
        void swap(SparseMatrix2D const & in);

    private:
        // Number of rows
        size_type     nrows_;

        // Number of columns
        size_type     ncols_;

        // row-major format
        mutable Row_t data_;

        // Check whether the matrix has already been converted to the
        // compresses row storage format
        mutable bool  finalized_;

        /* Compressed row storage format
         * 
         * elements_ contains the non-zero elements in sequential order.
         * columns_ contains the column for the element.
         * nelements_: Contains the increasing number of elements per row.
         * 
         * The number of rows can be determined from nelements_: 
         * nelements_.size() == #rows + 1
         */
        mutable std::vector<double>    elements_;
        mutable std::vector<size_type> columns_;
        mutable std::vector<size_type> nelements_;
    };

} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
