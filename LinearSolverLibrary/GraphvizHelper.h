/*
 * Name  : GraphvizHelper.h
 * Path  :
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/29/2015
 */
#pragma once

#include "DeclSpec.h"


 // forward declarations
namespace LinearSolverLibrary_NS {
    class IAMGStrengthPolicy;
}


namespace LinearSolverLibrary_NS {

    class LINEAR_SOLVER_LIBRARY_DECL_SYMBOLS GraphvizHelper {
    public:
        static void exportToGraphviz(LinAlg_NS::IMatrix2D::size_type nvertices, IAMGStrengthPolicy const & strength_policy, std::string filename);
    };

} // LinearSolverLibrary_NS
