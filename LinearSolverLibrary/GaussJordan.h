/*
 * Name  : Gauss
 * Path  : 
 * Use   : Implements sparse linear solver algorithm MINRES
 * Author: Sven Schmidt
 * Date  : 08/03/2013
 */
#pragma once

#include "DeclSpec.h"

#include "LinAlg/entities.h"

#include <vector>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS GaussJordan final {
    public:
        typedef std::tuple<bool, LinAlg_NS::Matrix2D, LinAlg_NS::Vector> Return_t;
        typedef LinAlg_NS::IMatrix2D::size_type size_type;

    public:
        Return_t solve(LinAlg_NS::Matrix2D const & A, LinAlg_NS::Vector const & b);

    private:
        void initializePivoting(size_type rows) const;
        size_type getPivotElementsRowIndex(LinAlg_NS::Matrix2D const & A, size_type column_index);
        void adjustPivotingMap(size_type source_row, size_type dest_row) const;
        void rearrangeDueToPivoting(LinAlg_NS::Matrix2D & A, LinAlg_NS::Matrix2D & AInverse, LinAlg_NS::Vector & rhs) const;
        void print(LinAlg_NS::Matrix2D const & A) const;
        size_type logicalToPhysicalRowIndex(size_type physical_row_index) const;
        size_type physicalToLogicalRowIndex(size_type logical_row_index) const;

    private:
        mutable std::vector<size_type> partial_pivoting_map_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
