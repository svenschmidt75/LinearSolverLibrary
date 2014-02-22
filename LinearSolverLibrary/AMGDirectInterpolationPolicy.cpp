#include "pch.h"

#include "AMGDirectInterpolationPolicy.h"
#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableCategorizer.h"
#include "VariableInfluenceAccessor.h"
#include "AMGStandardSplitting.h"
#include "AMGCoarseVariableIndexer.h"

using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


AMGDirectInterpolationPolicy::AMGDirectInterpolationPolicy() {}

bool
AMGDirectInterpolationPolicy::generate(SparseMatrix2D const & m) {






    // TODO SS: This is one object!
    AMGStandardCoarseningStrengthPolicy strength_policy{m};
    VariableCategorizer variable_categorizer{m.rows()};
    VariableInfluenceAccessor influence_accessor{strength_policy, variable_categorizer};
    AMGStandardSplitting splitting{m, influence_accessor, variable_categorizer};
    splitting.generateSplitting();








    // TODO SS: pass the above object here!
    ComputeInterpolationOperator(m, strength_policy, variable_categorizer);

    return true;
}

namespace {

    double
    ComputeNumerator(AMGDirectInterpolationPolicy::size_type fine_variable, ConstColumnIterator<SparseMatrix2D> && iterator) {
        double value = 0;
        for (; iterator; ++iterator) {
            if (iterator.column() == fine_variable)
                continue;
            double a_ik = *iterator;
            value += a_ik;
        }
        return value;
    }

    double
    ComputeDenumerator(AMGDirectInterpolationPolicy::size_type fine_variable, SparseMatrix2D const & m, IVariableSet const & interpolatory_set, VariableCategorizer const & variable_categorizer) {
        double value = 0;
        for (auto coarse_variable : interpolatory_set) {
            if (variable_categorizer.GetType(coarse_variable) != VariableCategorizer::Type::COARSE)
                continue;
            double a_ik = m(fine_variable, coarse_variable);
            value += a_ik;
        }
        return value;
    }

}

void
AMGDirectInterpolationPolicy::ComputeInterpolationOperator(SparseMatrix2D const & m, AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) {
    // The interpolation operator has one row for each variable (i.e. both
    // fine and coarse) and one column for each coarse variable.
    Interpolation_t interpolation_op;
    AMGCoarseVariableIndexer indexer{variable_categorizer};
    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m);
    while (row_it) {
        auto fine_variable = row_it.row();
        if (variable_categorizer.GetType(fine_variable) == VariableCategorizer::Type::COARSE) {
            // no interpolation needed
            auto cv = indexer.Index(fine_variable);
            interpolation_op[{fine_variable, cv}] = 1;
        } else {
            double a_num = ComputeNumerator(fine_variable, *row_it);
            auto interpolatory_set = strength_policy.GetInfluencedByVariables(fine_variable);
            if (interpolatory_set->size() == 0)
                throw std::runtime_error("AMGDirectInterpolationPolicy::ComputeInterpolationOperator: Fine variable has no strong connections to C variable");
            double a_denum = ComputeDenumerator(fine_variable, m, *interpolatory_set, variable_categorizer);
            double alpha = a_num / a_denum;
            double a_ii = m(fine_variable, fine_variable);
            for (auto coarse_variable : *interpolatory_set) {
                if (variable_categorizer.GetType(coarse_variable) != VariableCategorizer::Type::COARSE)
                    continue;
                double a_ik = m(fine_variable, coarse_variable);
                double w_ik = -alpha * a_ik / a_ii;
                auto cv = indexer.Index(coarse_variable);
                interpolation_op[{fine_variable, cv}] = w_ik;
            }
        }
        ++row_it;
    }
    CreateInterpolationOperator(m.rows(), indexer.NumberOfVariables(), interpolation_op);
}

void
AMGDirectInterpolationPolicy::CreateInterpolationOperator(size_type rows, size_type columns, Interpolation_t const & interpolation_op) {
    interpolation_operator_ = SparseMatrix2D{rows, columns};
    for (auto const & item : interpolation_op) {
        auto fine_variable = item.first.first;
        auto coarse_variable = item.first.second;
        auto value = item.second;
        interpolation_operator_(fine_variable, coarse_variable) = value;
    }
    interpolation_operator_.finalize();
//    interpolation_operator_.print();
}

SparseMatrix2D
AMGDirectInterpolationPolicy::coarseLevelMatrix() const {
    return SparseMatrix2D{ 5 };
}

SparseMatrix2D
AMGDirectInterpolationPolicy::prolongator() const {
    return SparseMatrix2D{5};
}

SparseMatrix2D
AMGDirectInterpolationPolicy::interpolator() const {
    return interpolation_operator_;
}
