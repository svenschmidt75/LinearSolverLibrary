#include "pch.h"

#include "MultiColor_internal.h"

#include "BucketList.h"
#include "BucketElement.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace internal_NS;


BucketList
MultiColor_internal::extractGraph(SparseMatrix2D const & m) {
    /* Extract dependencies of elements x_i on elements x_j, i.e.
     * if a(i,j) != 0 => x_i depends on x_j.
     * The BucketList contains each x_i as BucketElement, that
     * describes x_i's dependencies on other x_j.
     */
    SparseMatrix2D::size_type nrows = m.rows();
    SparseMatrix2D::size_type ncols = m.cols();

    std::vector<BucketElement::Ptr> bucket_elements(nrows);
    for (SparseMatrix2D::size_type row = 0; row < nrows; ++row) {
        bucket_elements[row] = BucketElement::Ptr(new BucketElement(row));
    }


    BucketList bucket_list;

    for (SparseMatrix2D::size_type row = 0; row < nrows; ++row) {
        BucketElement::Ptr & be = bucket_elements[row];

        /* We generate an undirected graph here, i.e. we pick up
         * a(i,j) as well as a(j,i). If only outgoing dependencies
         * are that matter, then change the col loop to
         * col < row, i.e. we only look at the lower-triangular
         * dependencies.
         */
        for (SparseMatrix2D::size_type col = 0; col < ncols; ++col) {
            if (m(row, col)) {
                BucketElement::Ptr dep = bucket_elements[col];

                // element 'row' depends on element 'col'
                be->dependsOn(dep);
            }
        }

        // insert element into BucketList
        bucket_list.insert(be);
    }

    return bucket_list;
}

std::vector<BucketList>
MultiColor_internal::decompose(BucketList bl) {
    typedef std::vector<BucketList> BucketListSet;

    // split the bucket elements such that all bucket elements in one set have no dependencies on
    // elements in the same list
    BucketListSet bl_done;
    BucketList bl_next;
    BucketList bl_loop;
    bl_loop = bl;

    auto bl_loop_start_it = bl_loop.begin();

    while (bl_loop.size() > 1) {
        //  check dependencies of first bucket element against all others
        // in same bucket
        auto bl_loop_current_it = bl_loop_start_it;

        BucketElement::Ptr const & be_first = *bl_loop_start_it;

        for (++bl_loop_current_it; bl_loop_current_it != bl_loop.end(); ) {
            BucketElement::Ptr const be_next = *bl_loop_current_it;

            // if there is an edge between elements be_first and be_next,
            // move e out of bl_loop into bl_next.
            bool split = false;

            BucketElement::const_iterator it_dep = be_first->findDependency(*be_next);
            if (it_dep != be_first->end())
                split = true;

            else {
                it_dep = be_next->findDependency(*be_first);
                if (it_dep != be_next->end())
                    split = true;
            }

            if (split) {
                ++bl_loop_current_it;
                bl_loop.remove(be_next);
                bl_next.insert(be_next);
            }
            else
                ++bl_loop_current_it;
        }

        // check next element in bucket
        bl_loop_start_it++;

        // all elements in current bucket decomposed?
        if (bl_loop_start_it == bl_loop.end()) {
            // bl_loop only contains elements that are independent
            bl_done.push_back(bl_loop);

            bl_loop = bl_next;
            bl_loop_start_it = bl_loop.begin();

            bl_next.clear();
        }
    }

    return bl_done;
}
