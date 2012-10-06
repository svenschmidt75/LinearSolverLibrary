#include "pch.h"

#include "BucketList.h"

using namespace LinearSolverLibrary_NS;
using namespace internal_NS;


BucketList::BucketList() {}

void
BucketList::insert(BucketElement::Ptr const & element) {
    data_.insert(element);
}

void
BucketList::remove(BucketElement::Ptr const & element) {
    data_.erase(element);
}

BucketList::iterator
BucketList::begin() const {
    return data_.begin();
}

BucketList::iterator
BucketList::end() const {
    return data_.end();
}

BucketList::const_iterator
BucketList::cbegin() const {
    return data_.cbegin();
}

BucketList::const_iterator
BucketList::cend() const {
    return data_.cend();
}

BucketList::const_iterator
BucketList::findElement(BucketElement::Ptr const & element) const {
    return data_.find(element);
}

BucketList::const_iterator
BucketList::findElement(size_type index) const {
    auto it = std::find_if(data_.cbegin(), data_.cend(), [index](BucketElement::Ptr const & element) {
        return element->prevIndex() == index;
    });

    return it;
}

BucketList::size_type
BucketList::size() const {
    return data_.size();
}

bool
BucketList::empty() const {
    return data_.empty();
}

void
BucketList::clear() {
    data_.clear();
}
