#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


class AMGCThenFReleaxationPolicyTest : public Test {
public:
    using size_type = IMatrix2D::size_type;
};


TEST_F(AMGCThenFReleaxationPolicyTest, TestSimpleDecompositionFor3By3Matrix) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;
    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    AMGStandardStrengthPolicy strength_policy{m};
    VariableCategorizer variable_categorizer{m.rows()};
    VariableInfluenceAccessor influence_accessor{strength_policy, variable_categorizer};
    AMGStandardCoarsening coarsening{m, influence_accessor, variable_categorizer};
    coarsening.coarsen();

    AMGDirectInterpolationPolicy interpolation_policy;
    interpolation_policy.Generate(m, strength_policy, variable_categorizer);


    AMGCThenFRelaxationPolicy relaxation_policy;
    MatrixDecomposition decomposition = relaxation_policy.Decompose(variable_categorizer);

    // check coarse variables
    auto variables = std::find_if(std::cbegin(decomposition), std::cend(decomposition), [](std::pair<size_type, std::set<size_type>> const & item) -> bool {
        return item.first == 0;
    });
    ASSERT_THAT(variables, Ne(std::cend(decomposition)));
    ASSERT_THAT(variables->second, ElementsAre(0, 2, 4, 6, 8));

    // check fine variables
    variables = std::find_if(std::cbegin(decomposition), std::cend(decomposition), [](std::pair<size_type, std::set<size_type>> const & item) -> bool {
        return item.first == 1;
    });
    ASSERT_THAT(variables, Ne(std::cend(decomposition)));
    ASSERT_THAT(variables->second, ElementsAre(1, 3, 5, 7));
}
