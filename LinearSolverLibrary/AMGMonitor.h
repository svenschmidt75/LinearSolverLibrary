/*
* Name  : AMGMonitor
* Path  :
* Use   : Settings for the iteration
* Author: Sven Schmidt
* Date  : 01/18/2014
*/
#pragma once


#include "Monitor.h"


namespace LinearSolverLibrary_NS {

    struct AMGMonitor : public Monitor {
        AMGMonitor() : nu1{1}, nu2{1} {}

        /********************
         * INPUT
         ********************/

        // If the size of the matrix falls below this limit,
        // we solve for it directly (via LU decomposition).
        int direct_solver_threshold;

        // number of pre-smooting iterations
        short nu1;

        // number of post-smooting iterations
        short nu2;
    };

} // LinearSolverLibrary_NS
