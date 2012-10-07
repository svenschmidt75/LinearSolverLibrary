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
#include <cstdint>
#include <map>
#include <set>


namespace LinAlg_NS {
    class SparseMatrix2D;
}


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class BucketList;


    class LINEARSOLVERLIBRARY_DECL_SYMBOLS MultiColor_internal {

        friend class MultiColorTest;

    public:
        typedef std::uint64_t color_t;
        typedef std::map<color_t, std::set<std::uint64_t>> ISO_t;

    public:
        static BucketList extractGraph(LinAlg_NS::SparseMatrix2D const & m);
        static void       computeColor(BucketList & bl);
        static ISO_t      decompose(BucketList & bl);
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS
