#include "pch.h"

#include "AMGStandardStrengthPolicy.h"
#include "VariableSet.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGStandardStrengthPolicy::AMGStandardStrengthPolicy(LinAlg_NS::SparseMatrix2D const & m)
    :
    Si_(new SparseMatrix2D(m.cols())),
    Sit_(new SparseMatrix2D(m.cols())),
    m_(m),
    eps_(0.25) {

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
                // skip diagonal element
                continue;
            double a_ij = std::abs(*column_it);
            max_value = std::max(max_value, a_ij);
        }
        return max_value;
    }

}

void
AMGStandardStrengthPolicy::computeConnectionsForVariable(IMatrix2D::size_type i, double max_element) {
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m_, i);
    BOOST_ASSERT_MSG(row_it.row() == i, "Index range error");
    auto column_it = *row_it;
    for (; column_it.isValid(); ++column_it) {
        auto j = column_it.column();
        if (j == i)
            // skip diagonal element
            continue;
        double a_ij = - *column_it;
        if (a_ij >= max_element) {
            // Variable i has a strong dependency on variable j as the coupling element a_ij
            // is large. Hence, algebraic smooth error varies slowly between i to j, and hence we
            // want to coarsen in that "direction".
            (*Si_)(i, j) = 1.0;

            // variable j strongly influences variable i
            (*Sit_)(j, i) = 1.0;
        }
    }
}

void
AMGStandardStrengthPolicy::computeConnections() {
    // TODO SS: This can be done in parallel for each fine variable i, as
    // only S(i, j) is modified with i = const per loop iteration.
    // Use thread private memory and at the end, assembly Si and Sit.
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

namespace {
    
    template<typename U, typename T>
    std::unique_ptr<U>
    convert(std::unique_ptr<T> & ptr) {
        return std::unique_ptr<U>(ptr.release());
    }
}

std::unique_ptr<IVariableSet>
AMGStandardStrengthPolicy::GetInfluencedByVariables(LinAlg_NS::IMatrix2D::size_type variable) const {
    common_NS::reporting::checkUppderBound(variable, m_.rows());
    // return the variables that strongly influence variable 'variable'
    // TODO SS: Use std::make_unique
    auto variable_set = std::make_unique<VariableSet>();
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(*Si_, variable);
    auto column_it = *row_it;
    for (; column_it.isValid(); ++column_it) {
        auto j = column_it.column();
        variable_set->add(j);
    }
    return convert<IVariableSet>(variable_set);
}

std::unique_ptr<IVariableSet>
AMGStandardStrengthPolicy::GetDependentOnVariables(LinAlg_NS::IMatrix2D::size_type variable) const {
    common_NS::reporting::checkUppderBound(variable, m_.rows());
    // return the variables that variable 'variable' strongly influences
    auto variable_set = std::make_unique<VariableSet>();
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(*Sit_, variable);
    auto column_it = *row_it;
    for (; column_it.isValid(); ++column_it) {
        auto j = column_it.column();
        variable_set->add(j);
    }
    return convert<IVariableSet>(variable_set);
}
