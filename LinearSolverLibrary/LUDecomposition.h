/*
 * Name  : LUDecomposition
 * Path  : 
 * Use   : Implements sparse linear solver algorithm MINRES
 * Author: Sven Schmidt
 * Date  : 08/03/2013
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/entities.h"

#include <vector>
#include <memory>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS LUDecomposition final {

        friend class LUDecompositionTest;

    public:
        typedef std::tuple<bool, LinAlg_NS::Matrix2D, LinAlg_NS::Vector> Return_t;
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        bool decompose(LinAlg_NS::Matrix2D const & A) const;
        LinAlg_NS::Vector solve(LinAlg_NS::Vector const & rhs) const;

    private:
        void initializePivoting(size_type rows) const;
        size_type getPivotElementsRowIndex(LinAlg_NS::Matrix2D const & A, size_type column_index) const;
        void adjustPivotingMap(size_type source_row, size_type dest_row) const;
        LinAlg_NS::Matrix2D rearrangeDueToPivoting() const;
        void print(LinAlg_NS::Matrix2D const & A) const;
        inline size_type logicalToPhysicalRowIndex(size_type physical_row_index) const;
        inline size_type physicalToLogicalRowIndex(size_type logical_row_index) const;
        void swapRows(LinAlg_NS::IMatrix2D::size_type row1, LinAlg_NS::IMatrix2D::size_type row2) const;
        LinAlg_NS::Vector forwardSubstitute(LinAlg_NS::Vector const & rhs ) const;
        LinAlg_NS::Vector backwardSubstitute(LinAlg_NS::Vector const & rhs ) const;

    private:
        mutable std::unique_ptr<LinAlg_NS::Matrix2D> LU_;
        mutable std::vector<size_type> partial_pivoting_map_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
