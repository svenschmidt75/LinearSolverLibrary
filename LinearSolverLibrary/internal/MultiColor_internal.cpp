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
    BucketList bucket_list;

    SparseMatrix2D::size_type nrows = m.rows();
    SparseMatrix2D::size_type ncols = m.cols();

    for (SparseMatrix2D::size_type row = 0; row < nrows; ++row) {
        BucketElement be(row);

        /* We generate an undirected graph here, i.e. we pick up
         * a(i,j) as well as a(j,i). If only outgoing dependencies
         * are that matter, then change the col loop to
         * col < row, i.e. we only look at the lower-triangular
         * dependencies.
         */
        for (SparseMatrix2D::size_type col = 0; col < ncols; ++col) {
            if (m(row, col))
                // element 'row' depends on element 'col'
                    be.insertDependency(col);
        }

        // insert element into BucketList
        bucket_list.insert(be);
    }

    return bucket_list;
}
