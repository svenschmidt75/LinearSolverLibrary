/*
 * Name  : BucketList
 * Path  : 
 * Use   : Contains lists of BucketElements.
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include <forward_list>


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class BucketElement;


    class BucketList {
    public:
        void insert(BucketElement const & in);
        void insert(BucketElement && in);

    private:
        typedef std::forward_list<BucketElement> DataType;
        DataType data_;
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS
