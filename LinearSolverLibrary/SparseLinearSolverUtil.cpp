#include "pch.h"

#include "SparseLinearSolverUtil.h"
#include "internal/MultiColor.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


bool
SparseLinearSolverUtil::isStrictlyDiagonallyDominant(SparseMatrix2D const & m) {
    /* Many (if not all) iterative linear solvers require that the
     * matrix m is strictly diagonally dominant for convergence.
     * Note: This is NOT a necessary, but not sufficient condition.
     * There are exceptions, i.e. matrices m that are not
     * diagonally dominant and yet the scheme converges.
     * Often, interchanging rows can bring a matrix into a strictly
     * diagonally format. Note that the same rows need to be interchanged
     * for both x and the r.h.s. vector b!
     */

    // Number of rows
    typedef decltype(m.nelements_.size()) size_type;
    size_type nrows = m.nelements_.size() - 1;

    for (size_type row = 0; row < nrows; ++row) {
        double a_ii = 0;
        bool has_diagonal_element = false;

        // Number of non-zero columns for this row
        size_type ncol = m.nelements_[row + 1] - m.nelements_[row];
        size_type offset = m.nelements_[row];

        double sum = 0;

        for (int icol = 0; icol < ncol; ++icol) {
            if (row == m.columns_[offset + icol]) {
                a_ii = m.elements_[offset + icol];
                has_diagonal_element = true;
                continue;
            }

            // add the total value of the off-diagonal elements in row 'row'
            double a_ij = m.elements_[offset + icol];
            sum += std::fabs(a_ij);
        }

        /* diagonally dominant is a per-row property */
        if (!has_diagonal_element || sum > std::fabs(a_ii))
            return false;
    }

    return true;
}

bool
SparseLinearSolverUtil::isVectorEqual(Vector const & v1, Vector const & v2, double tol = 1E-10) {
    if (v1.size() != v2.size())
        throw std::runtime_error("SparseLinearSolverUtil::isVectorEqual: Vectors have unequal size");

    double max_delta = std::numeric_limits<double>::min();

    auto ret = std::mismatch(v1.cbegin(), v1.cend(), v2.cbegin(), [tol, &max_delta](double d1, double d2){
        double delta = std::fabs(d1 - d2);
        max_delta = std::max(max_delta, delta);

        if (delta >= tol) {
            int a = 1;
            a++;
        }
        return delta < tol;
    });

    std::cout << "max_delta: " << max_delta << std::endl;

    return ret.first == v1.cend() && ret.second == v2.cend();
}

MatrixDecomposition
SparseLinearSolverUtil::multicolorDecomposition(SparseMatrix2D const & m) {
     return internal_NS::MultiColor::apply(m);
}


#if 0

function [x, error, total_iters] = ...
    tfqmr(x0, b, atv, params)
    % TFQMR solver for linear systems
    %
    % C. T. Kelley, December 28, 1994
    %
    % This code comes with no guarantee or warranty of any kind.
    %
    % function [x, error, total_iters]
%                    = tfqmr(x0, b, atv, params)
    %
    %
    % Input:        x0=initial iterate
    %               b=right hand side
    %               atv, a matrix-vector product routine
    %			atv must return Ax when x is input
    %			the format for atv is
    %                       function ax = atv(x)
    %               params = two dimensional vector to control iteration
    %                        params(1) = relative residual reduction factor
    %                        params(2) = max number of iterations
    %
    % Output:       x=solution
    %               error = vector of iteration residual norms
    %               total_iters = number of iterations
    %
    % 
    %

    %
    % initialization
    %
    n=length(b); errtol = params(1)*norm(b); kmax = params(2); error=[]; x=x0;
%
    if norm(x)~=0
        r=b-feval(atv,x);
    else
        r=b;
end
    error=[];
%
    u=zeros(n,2); y=zeros(n,2); w = r; y(:,1) = r; 
k=0; d=zeros(n,1); v=feval(atv,y(:,1)); u(:,1)=v;
theta=0; eta=0; tau=norm(r); error=[error,tau];
rho=tau*tau;
%
    % TFQMR iteration
    %
    while( k < kmax)
        k=k+1;
sigma=r'*v; 
    %
    if sigma==0
        error('TFQMR breakdown, sigma=0')
        end
        %
        alpha=rho/sigma;
%
    % 
    %
    for j=1:2
        %
        %   Compute y2 and u2 only if you have to
        %
        if j==2 
            y(:,2)=y(:,1)-alpha*v;
u(:,2)=feval(atv,y(:,2));
end
    m=2*k-2+j;
w=w-alpha*u(:,j);
d=y(:,j)+(theta*theta*eta/alpha)*d;
theta=norm(w)/tau; c=1/sqrt(1+theta*theta);
tau=tau*theta*c; eta=c*c*alpha;
x=x+eta*d;
%
    %   Try to terminate the iteration at each pass through the loop
    %
    if tau*sqrt(m+1) <= errtol
        error=[error, tau];
total_iters=k;
return
    end
    end
    %
    %
    %
    if rho==0
        error('TFQMR breakdown, rho=0')
        end
        %
        rhon=r'*w; beta=rhon/rho; rho=rhon;
        y(:,1)=w + beta*y(:,2);
u(:,1)=feval(atv,y(:,1));
v=u(:,1)+beta*(u(:,2)+beta*v);
error=[error, tau];
total_iters=k;
end
#endif