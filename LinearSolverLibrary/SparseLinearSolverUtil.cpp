#include "pch.h"

#include "SparseLinearSolverUtil.h"


using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


bool
SparseLinearSolverUtil::isDiagonallyDominant(SparseMatrix2D const & m) {
    /* Many (if not all) iterative linear solvers require that the
     * matrix m is diagonally dominant for conversion.
     * It is a necessary, but not sufficient condition (?).
     * There are exceptions, i.e. matrices m that are not
     * diagonally dominant and yet the scheme converges.
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

    auto ret = std::mismatch(v1.cbegin(), v1.cend(), v2.cbegin(), [tol](double d1, double d2){
        double delta = std::fabs(d1 - d2);
        return delta < tol;
    });

    return ret.first == v1.cend() && ret.second == v2.cend();
}


MultiColor(SparseMatrix2D const & m) {
    /* Reorder the rows of m such that we create a decomposition of x_i's into independent sets. Note that
     * reordering of rows does not change b in Ax = b (see Saad, p. 78, em 3.1). The reordered matrix will
     * have the first n rows s.t. there is no dependence on x^{k+1}_{i} from the same iteration k+1
     * (i.e. no elements on the lower-left part).
     */

    // move all buckets with their dependencies on other elements into the bucket
    bucket b_init = {0}
    for each row in Matrix m
        b_i = new bucket_entry b
        b->index = row;
        for all columns j in Matrix m(row,j)
            b->dependsOn(bucket_entry(m(row, j)))
        end all
    end for each


    bucket b_done = {0}
    bucket b_next = {0}
    bucket b_loop = b_init
    for each bucket_element(i) in b_loop
        for all dependencies dep_j in bucket_element(i)
            if (bucket_entry(dep(j)).bucket == bucket_element(i).bucket) {
                // in same bucket, split
                remove bucket_entry(dep(j)) from b_loop
                insert bucket_entry(dep(j)) into b_next
            }
        end for all

        b_done.insert(b_loop)
        b_loop = b_next
        b_next = {0}
    end for each


    // enumerate bucket elements
    int = 0;
    for each bucket_list b_list in b_done
        for each bucket_element(i) in b_list
            bucket_element(i).setIndex = i++;
        end for each
    end for each


    // construct new matrix m
    for each bucket_list in d_done
        for all dependencies dep_j in bucket_element(i)
            int old_row = bucket_element(i).index;
            int old_col = bucket_element(dep_j).index;
            double value = m(old_row, old_col)

            int new_row = bucket_element(i).bucket;
            int new_col = bucket_element(dep_j).bucket;

            new_m(new_row, new_col) = value;
        end for all
    end for each
}
