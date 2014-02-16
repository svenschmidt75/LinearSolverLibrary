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

    // compute interpolation operator
//    AMGDirectInterpolation

    return true;
}

void
AMGDirectInterpolationPolicy::ComputeInterpolationOperator(SparseMatrix2D const & m, AMGStandardCoarseningStrengthPolicy const & strength_policy, VariableCategorizer const & variable_categorizer) {
    using size_type = IMatrix2D::size_type;

    // The interpolation operator has as many rows as there are
    // fine grid variables.
    SparseMatrix2D interpolation_operator{m.rows()};

    ConstRowColumnIterator<SparseMatrix2D> row_it = MatrixIterators::getConstRowColumnIterator(m);
    while (row_it) {
        auto fine_variable = row_it.row();
        auto interpolatory_set = strength_policy.GetInfluencedByVariables(fine_variable);

        double a_num = 0;
        double a_denum = 0;

        
        auto col_it = *row_it;
        for (; col_it; ++col_it) {
            if (col_it.column() == fine_variable)
                continue;
            double a_ik = *col_it;
            a_num += a_ik;
        }



        for (auto coarse_variable : *interpolatory_set) {
            double a_ik = m(fine_variable, coarse_variable);
            a_denum += a_ik;
        }

        double alpha = a_num / a_denum;
        double a_ii = m(fine_variable, fine_variable);

        for (auto coarse_variable : *interpolatory_set) {
            double a_ik = m(fine_variable, coarse_variable);
            double w_ik = - alpha * a_ik / a_ii;
            interpolation_operator(fine_variable, coarse_variable) = w_ik;
        }

        ++row_it;
    }
    interpolation_operator.finalize();
    interpolation_operator.print();
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
    return SparseMatrix2D{ 5 };
}
