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
                BucketElement::Ptr const & dep = bucket_elements[col];

                // element 'row' depends on element 'col'
                be->dependsOn(dep);
            }
        }

        // insert element into BucketList
        bucket_list.insert(be);
    }

    return bucket_list;
}

void
MultiColor_internal::computeColor(BucketList & bl) {
    /* Implements algorithm 3.6, page 87, of Saad.
     * The resulting multimap contains
     * color -> {index of x_i}
     */

    for (auto & be : bl) {
        std::vector<short> equ_used(bl.size(), 0);

        // check all of be's dependencies
        for (auto & dep : *be) {
            // get the color of the dependent element
            if (dep->prevIndex() != be->prevIndex()) {
                color_t color = dep->color();
                equ_used[color] = 1;
            }
        }

        // find the minimum color > 0 of element be
        color_t min_color = 1;
        while (equ_used[min_color])
            min_color++;

        // assign color
        be->color(min_color);
    }
}

MultiColor_internal::ISO_t
MultiColor_internal::decompose(BucketList & bl) {
    /* Implements algorithm 3.6, page 87, of Saad.
     * The resulting multimap contains
     * color -> {index of x_i}
     */
    ISO_t independent_decomposition;

    computeColor(bl);

    for (auto & be : bl) {
        // get color of element be
        color_t min_color = be->color();
        independent_decomposition[min_color].insert(be->prevIndex());
    }

    return independent_decomposition;
}
