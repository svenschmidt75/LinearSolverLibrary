/*
 * Name  : BucketList
 * Path  : 
 * Use   : Contains lists of BucketElements.
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include "BucketElement.h"

#include <forward_list>
#include <cstdint>


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class BucketList {
    private:
        typedef std::forward_list<BucketElement> DataType;

    public:
        typedef std::uint64_t size_type;
        typedef DataType::const_iterator iterator;

    public:
        void insert(BucketElement const & in);
        void insert(BucketElement && in);
        void remove(BucketElement const & in);

        iterator begin() const;
        iterator end() const;

        iterator findElement(size_type element);

        size_type size() const;

        bool empty() const;

        void clear();

    private:
        DataType data_;
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS
