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
        BucketElement & be = *bucket_elements[row];

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
                be.dependsOn(dep);
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

    while (!bl_loop.empty()) {

        while (bl_loop.size() > 1) {
            //  check dependencies of first bucket element against all others
            // in same bucket
            BucketList::iterator it = bl_loop.begin();
            BucketElement const & bl_first = *it;

            for (++it; it != bl_loop.end(); ) {
                BucketElement const & e = *it;

                // if there is an edge between elements bl_first and e,
                // move e out of bl_loop into bl_next.
                bool split = false;

                BucketElement::const_iterator it_dep = bl_first.findDependency(e);
                if (it_dep != bl_first.end())
                    split = true;

                else {
                    it_dep = e.findDependency(bl_first);
                    if (it_dep != e.end())
                        split = true;
                }

                if (split) {
                    ++it;
                    bl_loop.remove(e);
                    bl_next.insert(e);
                }
                else
                    ++it;
            }

            // bl_loop only contains elements that are independent
            bl_done.push_back(bl_loop);

            bl_loop = bl_next;
            bl_next.clear();
        }
    }

    return bl_done;
}