/*
 * Name  : BucketList
 * Path  : 
 * Use   : Contains lists of BucketElements.
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include "..\DeclSpec.h"

#include "BucketElement.h"


#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS BucketList {
    private:
        typedef std::set<BucketElement::Ptr, BucketElement::comparitor> DataType;

    public:
        typedef std::uint64_t size_type;
        typedef DataType::const_iterator const_iterator;
        typedef DataType::iterator iterator;

    public:
        BucketList();

        void           insert(BucketElement::Ptr const & element);
        void           remove(BucketElement::Ptr const & element);

        iterator       begin() const;
        iterator       end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

        const_iterator findElement(BucketElement::Ptr const & element) const;
        const_iterator findElement(size_type index) const;

        size_type      size() const;

        bool           empty() const;

        void           clear();

    private:
        DataType data_;
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS

#pragma warning(default:4251)
