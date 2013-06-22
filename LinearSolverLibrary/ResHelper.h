/*
 * Name  : ResHelper
 * Path  : 
 * Use   : Helper class containing utility functions for minres/gmres.
 * Author: Sven Schmidt
 * Date  : 06/15/2013
 */
#pragma once

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"


namespace LinearSolverLibrary_NS {

    class ResHelper {
    public:
        static void GeneratePlaneRotation(double dx, double dy, double & cs, double & sn);
        static void ApplyPlaneRotation(double & dx, double & dy, double cs, double sn);
    };

} // LinearSolverLibrary_NS
