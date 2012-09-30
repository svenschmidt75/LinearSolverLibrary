#include "pch.h"

#include "BucketElement.h"


using namespace LinearSolverLibrary_NS;
using namespace internal_NS;


BucketElement::BucketElement(size_type element_index)
    :
    prev_element_index_(element_index) {}

BucketElement::const_iterator
BucketElement::cbegin() const {
    return dependencies_.cbegin();
}

BucketElement::const_iterator
BucketElement::cend() const {
    return dependencies_.cend();
}

BucketElement::iterator
BucketElement::begin() const {
    return dependencies_.begin();
}

BucketElement::iterator
BucketElement::end() const {
    return dependencies_.end();
}

void
BucketElement::dependsOn(Ptr element) {
    dependencies_.insert(element);
}

BucketElement::const_iterator
BucketElement::findDependency(BucketElement const & element) const {
    for (auto it = dependencies_.cbegin(); it != dependencies_.cend(); ++it) {
        if ((*it)->prevIndex() == element.prevIndex())
            return it;
    }
    return dependencies_.cend();
}

bool
BucketElement::findDependency(size_type index) const {
    auto it = std::find_if(dependencies_.cbegin(), dependencies_.cend(), [index](BucketElement::Ptr const & element) {
        return element->prevIndex() == index;
    });

    return it != dependencies_.end();
}

void
BucketElement::index(size_type index) {
    element_index_ = index;
}

BucketElement::size_type
BucketElement::index() const {
    return element_index_;
}

BucketElement::size_type
BucketElement::prevIndex() const {
    return prev_element_index_;
}
