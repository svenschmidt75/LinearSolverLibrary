#include "pch.h"

#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableSet.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGStandardCoarseningStrengthPolicy::AMGStandardCoarseningStrengthPolicy(LinAlg_NS::SparseMatrix2D const & m)
    : m_(m), Si_(new SparseMatrix2D(m.cols())), Sit_(new SparseMatrix2D(m.cols())), eps_(0.25) {

    computeConnections();
}

namespace {

    double computeMaxElementForRow(SparseMatrix2D const & m, IMatrix2D::size_type row) {
        ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m, row);
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
AMGStandardCoarseningStrengthPolicy::computeConnectionsForVariable(IMatrix2D::size_type i, double max_element) {
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m_, i);
    BOOST_ASSERT_MSG(row_it.row() == i, "Index range error");
    auto column_it = *row_it;
    for (; column_it.isValid(); ++column_it) {
        auto j = column_it.column();
        if (j == i)
            continue;
        double matrix_value = - *column_it;
        if (matrix_value >= max_element) {
            // variable i has a strong dependency on variable j
            (*Si_)(i, j) = 1.0;

            // variable j strongly influences on variable i
            (*Sit_)(j, i) = 1.0;
        }
    }

}

void
AMGStandardCoarseningStrengthPolicy::computeConnections() {
    for (IMatrix2D::size_type i = 0; i < m_.rows(); ++i) {
        auto max_element = computeMaxElementForRow(m_, i);
        if (max_element == 0)
            // Variable i has no strong connections to other variables,
            // hence it will become an F variable.
            continue;
        max_element *= eps_;
        computeConnectionsForVariable(i, max_element);
    }
    Si_->finalize();
    Sit_->finalize();
}

std::shared_ptr<IVariableSet>
AMGStandardCoarseningStrengthPolicy::GetInfluencedByVariables(LinAlg_NS::IMatrix2D::size_type variable) const {
    common_NS::reporting::checkUppderBound(variable, m_.rows());
    // return the variables that strongly influence variable 'variable'
    // TODO SS: Use std::make_unique
    std::shared_ptr<VariableSet> variable_set = std::make_shared<VariableSet>();
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(*Si_, variable);
    auto column_it = *row_it;
    for (; column_it.isValid(); ++column_it) {
        auto j = column_it.column();
        variable_set->add(j);
    }
    return variable_set;
}

std::shared_ptr<IVariableSet>
AMGStandardCoarseningStrengthPolicy::GetDependentOnVariables(LinAlg_NS::IMatrix2D::size_type variable) const {
    common_NS::reporting::checkUppderBound(variable, m_.rows());
    // return the variables that variable 'variable' strongly influences
    std::shared_ptr<VariableSet> variable_set = std::make_shared<VariableSet>();
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(*Sit_, variable);
    auto column_it = *row_it;
    for (; column_it.isValid(); ++column_it) {
        auto j = column_it.column();
        variable_set->add(j);
    }
    return variable_set;
}
