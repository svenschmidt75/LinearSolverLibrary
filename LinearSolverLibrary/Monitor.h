/*
* Name  : Monitor
* Path  :
* Use   : Settings for the iteration
* Author: Sven Schmidt
* Date  : 04/27/2014
*/
#pragma once


namespace LinearSolverLibrary_NS {

    struct Monitor {
        Monitor()
            :
            nmax_iterations{10000},
            required_tolerance{1E-16},
            nstall_iterations{50},
            verbosity{0} {}

        /********************
         * INPUT
         ********************/

        // Maximum number of iterations after which the iteration
        // is terminated.
        size_t nmax_iterations;

        // Number of iterations for detecting stalls.
        int nstall_iterations;

        double required_tolerance;

        int verbosity;

        /********************
         * OUTPUT
         ********************/

        // Number of iterations performed in the numeric solver.
        int iterations;

        // Residual at which the iteration has finished.
        double residual;

        double achieved_tolerance;
    };

} // LinearSolverLibrary_NS
