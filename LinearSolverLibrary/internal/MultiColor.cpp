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
    * reordering of rows does not change b in Ax = b (see Saad, p. 78, ex 3.1). The reordered matrix will
    * have the first n rows s.t. there is no dependence on x^{k+1}_{i} from the same iteration k+1
    * (i.e. no elements on the lower-left part).
    */

    // move all bucket elements with their dependencies on other bucket elements into the bucket list
    BucketList bucket_list = MultiColor_internal::extractGraph(m);

    // create the relation color -> {x_i}
    auto decomposed_buckets = MultiColor_internal::decompose(bucket_list);


    MatrixDecomposition mdecomp(decomposed_buckets);
    return mdecomp;
}
