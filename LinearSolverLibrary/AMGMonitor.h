/*
* Name  : AMGMonitor
* Path  :
* Use   : Settings for the iteration
* Author: Sven Schmidt
* Date  : 01/18/2014
*/
#pragma once


namespace LinearSolverLibrary_NS {

    struct AMGMonitor {
        // Input
        size_t number_of_max_iterations;
        double tolerance;

        // If the size of the matrix falls below this limit,
        // we solve for it directly (via LU decomposition).
        int direct_solver_threshold_;

        // Output
        size_t number_of_iterations_needed;
    };

} // LinearSolverLibrary_NS
