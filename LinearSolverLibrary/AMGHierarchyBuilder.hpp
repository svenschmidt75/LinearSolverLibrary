/*
* Name  : AMGHierarchyBuilder
* Path  :
* Use   : Generates AMG hierarchy
* Author: Sven Schmidt
* Date  : 02/01/2014
*/
#pragma once

#include "AMGLevel.h"
#include "AMGCThenFRelaxationPolicy.hpp"
#include "LinearSolverLibrary/IAMGStandardStrengthPolicy.h"
#include "AMGStandardCoarsening.h"


namespace LinearSolverLibrary_NS {


    // forward declarations
    struct AMGMonitor;


    template<typename AMGCoarseningStrategy, typename AMGInterpolationPolicy, typename AMGRelaxationPolicy = AMGCThenFRelaxationPolicy>
    class AMGHierarchyBuilder {
    public:
        AMGHierarchyBuilder(AMGMonitor const & monitor)
            :
            monitor_{monitor},
            current_level_index_{0} {}

        AMGHierarchyBuilder(AMGHierarchyBuilder const &) = delete;
        AMGHierarchyBuilder & operator=(AMGHierarchyBuilder const &) = delete;

        std::vector<AMGLevel>
        build(SparseMatrix2D const & m) const {
            AMGInterpolationPolicy interpolation_policy;
            AMGRelaxationPolicy relaxation_policy;
            
			int const direct_solver_threshold = monitor_.direct_solver_threshold;

            AMGLevel * current_level = CreateAMGLevel();
            current_level->m = m;

            std::shared_ptr<LinAlg_NS::SparseMatrix2D> interpolator{nullptr};

            while (SolveWithDirectMethod(GetCurrentLevel()->m, direct_solver_threshold) == false) {
                current_level = GetCurrentLevel();

                if (interpolator) {
                    current_level->interpolator = interpolator;
                    interpolator = nullptr;
                }

                // coarsen this level
                AMGStandardStrengthPolicy strength_policy{current_level->m};
                VariableCategorizer variable_categorizer{current_level->m.rows()};
                VariableInfluenceAccessor influence_accessor{strength_policy, variable_categorizer};
                AMGCoarseningStrategy coarsening{current_level->m, influence_accessor, variable_categorizer};
                coarsening.coarsen();

                if (interpolation_policy.Generate(current_level->m, strength_policy, variable_categorizer) == false) {
                    // coarsening failed
                    if (monitor_.verbosity > 1)
                        std::cout << "AMGHierarchyBuilder: Cannot coarse past level " << current_level_index_ << std::endl;
                    break;
                }

                current_level->restrictor = std::make_shared<SparseMatrix2D>(interpolation_policy.Restrictor());
                current_level->variableDecomposition = relaxation_policy.Decompose(variable_categorizer);

                AMGLevel * next_level = CreateAMGLevel();
                next_level->m = interpolation_policy.GalerkinOperator();

                // interpolation operator to this level from the next
                interpolator = std::make_shared<SparseMatrix2D>(interpolation_policy.Interpolator());

                MoveToNextLevel();
            }

            if (interpolator) {
                GetCurrentLevel()->interpolator = interpolator;
                interpolator = nullptr;
            }

            // avoid copy construction here
            return std::move(amg_levels_);
        }

        AMGLevel *
        CreateAMGLevel() const {
            auto index = amg_levels_.size();
            amg_levels_.emplace_back();
            return &amg_levels_[index];
        }

        AMGLevel *
        GetCurrentLevel() const {
            return &amg_levels_[current_level_index_];
        }

        void
        MoveToNextLevel() const {
            current_level_index_++;
        }

        bool
        SolveWithDirectMethod(SparseMatrix2D const & m, int max_size) const {
            return m.cols() <= max_size;
        }

    private:
        AMGMonitor const &            monitor_;
        mutable std::vector<AMGLevel> amg_levels_;
        mutable int                   current_level_index_;
    };

} // LinearSolverLibrary_NS
