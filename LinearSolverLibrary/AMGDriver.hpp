/*
* Name  : AMGDriver
* Path  :
* Use   : Generates AMG hierarchy
* Author: Sven Schmidt
* Date  : 01/18/2014
*/
#pragma once


namespace LinearSolverLibrary_NS {

    template<typename COARSENING, typename INTERPOLATION, typename SMOOTHER, typename AMG_SCHEME>
    class AMGDriver {
    public:
        AMGDriver(LinAlg_NS::SparseMatrix2D const & m, LinAlg_NS::Vector const & b, AMGMonitor & monitor)
            :
            m_{m},
            b_{b},
            monitor_{monitor} {
        
            // setup the multigrid hierarchy
            AMGHierarchyBuilder<COARSENING, INTERPOLATION> amg_builder{m_};
            amg_levels_ = amg_builder.build();
        }

        LinAlg_NS::Vector
        solve(LinAlg_NS::Vector const & x_initial) const {
        }

    private:
        LinAlg_NS::SparseMatrix2D const & m_;
        LinAlg_NS::Vector const &         b_;
        AMGMonitor &                      monitor_;
        std::vector<AMGLevel>             amg_levels_;
    };

} // LinearSolverLibrary_NS
