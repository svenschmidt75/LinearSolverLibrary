#include "pch.h"

#include "AMGDirectInterpolationPolicy.h"
#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableCategorizer.h"
#include "VariableInfluenceAccessor.h"
#include "AMGStandardSplitting.h"

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

void
AMGDirectInterpolationPolicy::ComputeInterpolationOperator(SparseMatrix2D const & m, AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) {
    using size_type = IMatrix2D::size_type;

    // The interpolation operator has as many rows as there are
    // fine grid variables.


    variable_categorizer.print();

    m.print();


    std::map<std::pair<size_type, size_type>, double> interpolation_op;


    std::map<size_type, size_type> mapper;
    size_type index = 0;

    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m);
    while (row_it) {
        auto fine_variable = row_it.row();

        if (variable_categorizer.GetType(fine_variable) == VariableCategorizer::Type::COARSE) {
            // no interpolation needed
            auto it = mapper.find(fine_variable);
            if (it == std::cend(mapper))
                mapper[fine_variable] = index++;
            auto cv = mapper[fine_variable];
            interpolation_op[{fine_variable, cv}] = 1;
            ++row_it;
            continue;
        }



        double a_num = 0;
        
        auto col_it = *row_it;
        for (; col_it; ++col_it) {
            if (col_it.column() == fine_variable)
                continue;
            double a_ik = *col_it;
            a_num += a_ik;
        }



        auto interpolatory_set = strength_policy.GetInfluencedByVariables(fine_variable);

        double a_denum = 0;
        for (auto coarse_variable : *interpolatory_set) {
            if (variable_categorizer.GetType(coarse_variable) != VariableCategorizer::Type::COARSE)
                continue;
            double a_ik = m(fine_variable, coarse_variable);
            a_denum += a_ik;
        }

        double alpha = a_num / a_denum;
        double a_ii = m(fine_variable, fine_variable);

        for (auto coarse_variable : *interpolatory_set) {
            if (variable_categorizer.GetType(coarse_variable) != VariableCategorizer::Type::COARSE)
                continue;
            double a_ik = m(fine_variable, coarse_variable);
            double w_ik = - alpha * a_ik / a_ii;

            auto it = mapper.find(coarse_variable);
            if (it == std::cend(mapper))
                mapper[coarse_variable] = index++;

            auto cv = mapper[coarse_variable];
            interpolation_op[{fine_variable, cv}] = w_ik;
        }

        ++row_it;
    }

    interpolation_operator_ = SparseMatrix2D{m.rows(), index};

    // construct interpolation operator
    for (auto const & item : interpolation_op) {
        auto fine_variable = item.first.first;
        auto coarse_variable = item.first.second;
        auto value = item.second;
        interpolation_operator_(fine_variable, coarse_variable) = value;
    }

    interpolation_operator_.finalize();
    interpolation_operator_.print();
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
