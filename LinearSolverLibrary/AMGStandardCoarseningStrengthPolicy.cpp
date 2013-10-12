#include "pch.h"

#include "AMGStandardCoarseningStrengthPolicy.h"

#include "LinAlg/iterators.h"

#include "common/reporting.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGStandardCoarseningStrengthPolicy::AMGStandardCoarseningStrengthPolicy(LinAlg_NS::SparseMatrix2D const & m)
    : m_(m), strength_matrix_(new SparseMatrix2D(m.cols())), eps_(0.25) {

    computeConnections();
}

namespace {

    double computeMaxElementForRow(SparseMatrix2D const & m, IMatrix2D::size_type row) {
        ConstRowIterator<SparseMatrix2D> row_it = iterators::getConstRowIterator(m, row);
        BOOST_ASSERT_MSG(row_it.row() == row, "Index range error");
        auto column_it = *row_it;
        double max_value = 0;
        for (; column_it.isValid(); ++column_it) {
            if (column_it.column() == row)
                continue;
            double matrix_value = std::abs(*column_it);
            max_value = std::max(max_value, matrix_value);
        }
        return max_value;
    }

}

void
AMGStandardCoarseningStrengthPolicy::computeConnectionsForVariable(IMatrix2D::size_type row, double max_element) {
    ConstRowIterator<SparseMatrix2D> row_it = iterators::getConstRowIterator(m_, row);
    BOOST_ASSERT_MSG(row_it.row() == row, "Index range error");
    auto column_it = *row_it;
    for (; column_it.isValid(); ++column_it) {
        auto j = column_it.column();
        if (j == row)
            continue;
        double matrix_value = - *column_it;
        if (matrix_value > max_element)
            (*strength_matrix_)(row, j) = 1.0;
    }

}

void
AMGStandardCoarseningStrengthPolicy::computeConnections() {
    for (IMatrix2D::size_type row = 0; row < m_.rows(); ++row) {
        auto max_element = computeMaxElementForRow(m_, row);
        if (max_element == 0)
            // Variable 'row' has no strong connections to other variables,
            // it will become a F variable.
            continue;
        max_element *= eps_;
        computeConnectionsForVariable(row, max_element);
    }
    strength_matrix_->finalize();
}

bool
AMGStandardCoarseningStrengthPolicy::VariableDependsOn(LinAlg_NS::IMatrix2D::size_type source, LinAlg_NS::IMatrix2D::size_type dest) const {
    common_NS::reporting::checkUppderBound(source, m_.rows());
    common_NS::reporting::checkUppderBound(dest, m_.rows());
    return (*strength_matrix_)(source, dest) != 0;
}
