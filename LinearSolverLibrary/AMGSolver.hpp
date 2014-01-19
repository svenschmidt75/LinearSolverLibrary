/*
* Name  : AMGSolver
* Path  :
* Use   : Generates AMG hierarchy
* Author: Sven Schmidt
* Date  : 01/18/2014
*/
#pragma once


namespace LinearSolverLibrary_NS {

    template<typename AMGPolicy>
    class AMGSolver {
    public:
        AMGSolver(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & b, AMGMonitor & monitor)
            :
            m_{m},
            b_{b},
            monitor_{monitor} {
        
            // setup the multigrid hierarchy
            AMGHierarchyBuilder<AMGPolicy> amg_builder{m_};
            amg_levels_ = amg_builder.build();
        }

        template<typename AMG_CYCLE_SCHEME = AMGVCYCLE>
        LinAlg_NS::Vector
        solve(LinAlg_NS::Vector const & x_initial) const {
            size_type level = 0;
            size_type max_level = amg_levels_.size() - 1;
            amg_levels_[0].b_ = b_;

            AMG_CYCLE_SCHEME cycle_scheme{max_level};

            level = cycle_scheme.level();
            while (level <= max_level) {
                auto & amg_level = amg_levels_[level];
                if (level == max_level) {
                    // solve directly via lu decomposition
                    amg_level.b_ = LUSolve(amg_level.A_, amg_level.b_);
                    cycle_scheme.next();
                    level = cycle_scheme.level;
                    continue;
                }
                Vector & x = pre_smooth(amg_level.A, amg_level.b);
                if (level < max_level) {
                    auto & next_amg_level = amg_levels_[level + 1];
                    residual = amg_level.b - amg_level.A * x;
                    residual = amg_level.restrict(residual);
                    next_amg_level.b = residual;
                    level++;
                    continue;
                }

                x = amg_level.prolongate(x);

                x = post_smooth(amg_level.A, x, amg_level.b);
            }
            return x;
        }

        LinAlg_NS::Vector
        solveFMG(LinAlg_NS::Vector const & x_initial) const {
        }

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
        LinAlg_NS::Vector const &         b_;
        AMGMonitor &                      monitor_;
        std::vector<AMGLevel>             amg_levels_;
    };


    class AMGLevel {
    public:

    private:
        LinAlg_NS::SparseMatrix2D prolongator;
        LinAlg_NS::SparseMatrix2D interpolator;
        LinAlg_NS::SparseMatrix2D A_;
        LinAlg_NS::Vector         b_;
        LinAlg_NS::Vector         x_;
    };


} // LinearSolverLibrary_NS
