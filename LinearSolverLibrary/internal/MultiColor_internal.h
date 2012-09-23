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


namespace LinAlg_NS {
    class SparseMatrix2D;
}


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class BucketList;


    class MultiColor_internal {

        friend class MultiColorTest;

    public:
        static BucketList extractGraph(LinAlg_NS::SparseMatrix2D const & m);

    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS
