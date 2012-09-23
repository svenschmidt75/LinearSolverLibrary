/*
 * Name  : BucketList
 * Path  : 
 * Use   : Contains lists of BucketElements.
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include <forward_list>
#include <cstdint>


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class BucketElement;


    class BucketList {
    private:
        typedef std::forward_list<BucketElement> DataType;

    public:
        typedef std::uint64_t size_type;
        typedef DataType::const_iterator iterator;

    public:
        void insert(BucketElement const & in);
        void insert(BucketElement && in);

        iterator begin() const;
        iterator end() const;

        iterator findElement(size_type element);

    private:
        DataType data_;
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS
