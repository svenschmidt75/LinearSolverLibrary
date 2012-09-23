#include "pch.h"

#include "MultiColor.h"

#include "internal/MultiColor_internal.h"
#include "internal/BucketList.h"


using namespace LinearSolverLibrary_NS;
using namespace internal_NS;
using namespace LinAlg_NS;


SparseMatrix2D
MultiColor::apply(SparseMatrix2D const & m) {
   /* Reorder the rows of m such that we create a decomposition of x_i's into independent sets. Note that
     * reordering of rows does not change b in Ax = b (see Saad, p. 78, em 3.1). The reordered matrix will
     * have the first n rows s.t. there is no dependence on x^{k+1}_{i} from the same iteration k+1
     * (i.e. no elements on the lower-left part).
     */

    // move all buckets with their dependencies on other elements into the bucket
    BucketList bucket_list = MultiColor_internal::extractGraph(m);








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


    /* 
    
    
    
    
    
    */




}

