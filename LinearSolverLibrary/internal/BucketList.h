/*
 * Name  : BucketList
 * Path  : 
 * Use   : Contains lists of BucketLists.
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include "pch.h"


namespace internal {

    class BucketList {
    public:


        void insert(BucketList const & in);
        void insert(BucketList && in);

    private:
        typedef std::forward_list<BucketList> DataType;
        DataType data_;
    };
}
