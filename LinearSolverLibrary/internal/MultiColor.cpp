#include "pch.h"

#include "MultiColor.h"
#include "MatrixDecomposition.h"

#include "internal/MultiColor_internal.h"
#include "internal/BucketList.h"


using namespace LinearSolverLibrary_NS;
using namespace internal_NS;
using namespace LinAlg_NS;


MatrixDecomposition
MultiColor::apply(SparseMatrix2D const & m) {
   /* Reorder the rows of m such that we create a decomposition of x_i's into independent sets. Note that
     * reordering of rows does not change b in Ax = b (see Saad, p. 78, em 3.1). The reordered matrix will
     * have the first n rows s.t. there is no dependence on x^{k+1}_{i} from the same iteration k+1
     * (i.e. no elements on the lower-left part).
     */

    // move all bucket elements with their dependencies on other bucket elements into the bucket list
    BucketList bucket_list = MultiColor_internal::extractGraph(m);


    // split the bucket elements such that all bucket elements in one set have no dependencies on
    // elements in the same list
    auto decomposed_buckets = MultiColor_internal::decompose(bucket_list);


    // enumerate bucket elements
    BucketElement::size_type index = 0;
    for (auto & bl : decomposed_buckets) {
        for (auto & e : bl) {
            e->index(index++);
        }
    }

    // construct set of independent elements
    std::vector<MatrixDecomposition::ISType> is_data;
    for (auto & bl : decomposed_buckets) {
        MatrixDecomposition::ISType data;

        for (auto & e : bl) {
            data.push_back(e->index());
        }

        is_data.push_back(data);
    }



    // construct new matrix m
    SparseMatrix2D new_m(m.cols());

    for (auto & bl : decomposed_buckets) {
        for (auto & e : bl) {
            SparseMatrix2D::size_type old_row = e->prevIndex();

            // all dependencies
            for (auto & dep : *e) {
                SparseMatrix2D::size_type old_col = dep->prevIndex();

                double value = m(old_row, old_col);

                SparseMatrix2D::size_type new_row = e->index();
                SparseMatrix2D::size_type new_col = dep->index();

                new_m(new_row, new_col) = value;
            }
        }
    }

    new_m.finalize();

    MatrixDecomposition mdecomp(new_m, is_data);
    return mdecomp;
}
