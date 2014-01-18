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
        size_t number_of_max_iterations;
        size_t number_of_iterations_needed;
        double tolerance;
    };

} // LinearSolverLibrary_NS
