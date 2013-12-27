#include "pch.h"

#include "ResHelper.h"


namespace LinearSolverLibrary_NS {

    /*
 * Computes the Givens rotation associated with scalars a and b such that
 * 
 * [  c   s ]T [a] = [r]
 * [ -s   c ]  [b]   [0]
 *
 * Reference:
 *	G. H. Golub and C. F. Van Loan, "Matrix Computations", 
 *	Johns Hopkins, Baltimore, 3rd ed. 1996.
 * 	Algorithm 5.1.3, p. 216.
 */
void
ResHelper::GeneratePlaneRotation(double dx, double dy, double & cs, double & sn) {
    if (dy == 0.0) {
        cs = 1.0;
        sn = 0.0;
    } else if (abs(dy) > abs(dx)) {
        double temp = dx / dy;
        sn = 1.0 / std::sqrt(1.0 + temp * temp);
        cs = temp * sn;
    } else {
        double temp = dy / dx;
        cs = 1.0 / std::sqrt(1.0 + temp * temp);
        sn = temp * cs;
    }
}

void
ResHelper::ApplyPlaneRotation(double & dx, double & dy, double cs, double sn) {
    // apply Givens rotation
    double tmp = cs * dx + sn * dy;
    dy = -sn * dx + cs * dy;
    dx = tmp;
}

} // LinearSolverLibrary_NS
