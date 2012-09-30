/*
 * Name  : MultiColor_internal
 * Path  : 
 * Use   : Operations that are private to the MultiColor
 *         implementation. They are not implemented in
 *         a private namespace in order to test these.
 * Author: Sven Schmidt
 * Date  : 09/23/2012
 */
#pragma once

#include "..\DeclSpec.h"

#include <vector>


namespace LinAlg_NS {
    class SparseMatrix2D;
}


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class BucketList;


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS MultiColor_internal {

        friend class MultiColorTest;

    public:
        static BucketList              extractGraph(LinAlg_NS::SparseMatrix2D const & m);
        static std::vector<BucketList> decompose(BucketList bl);

    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS
