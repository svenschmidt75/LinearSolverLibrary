#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"

#include "LinearSolverLibrary/AMGHierarchyBuilder.hpp"
#include "LinearSolverLibrary/AMGMonitor.h"
#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"
#include "LinearSolverLibrary/AMGCThenFRelaxationPolicy.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


class AMGCThenFReleaxationPolicyTest : public Test {
public:
    void SetUp() override {
        MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
        stencil << 0, -1,  0,
                  -1,  4, -1,
                   0, -1,  0;
        SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

        AMGDirectInterpolationPolicy splitting_policy;
        AMGMonitor monitor;
        monitor.direct_solver_threshold_ = 3;
        AMGHierarchyBuilder<AMGDirectInterpolationPolicy> builder{monitor};
        amg_levels = builder.build(m);
    }

public:
    std::vector<AMGLevel> amg_levels;
};


TEST_F(AMGCThenFReleaxationPolicyTest, Test1) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 0, -1, 0,
        -1, 4, -1,
        0, -1, 0;
    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    AMGDirectInterpolationPolicy interpolation_policy;
    interpolation_policy.Generate(m);


    AMGCThenFRelaxationPolicy relaxation_policy;
    MatrixDecomposition decomposition = relaxation_policy.Decompose(interpolation_policy);
    
//    ASSERT_EQ(amg_levels.size(), 3);
}
