/*
* Name  : IForwardIteratorLogic
* Path  :
* Use   : Helper functions that know about the internals of the data structure being
          iterated over. Used to implement ForwardIterator with.
* Author: Sven Schmidt
* Date  : 10/26/2013
*/
#pragma once

#include <memory>


namespace common_NS {

    template<typename T>
    class IInputIteratorLogic {
    public:
        virtual ~IInputIteratorLogic() {}

        virtual T                                       get() const = 0;
        virtual void                                    next() = 0;
        virtual bool                                    equal(IInputIteratorLogic const & in) const = 0;
        virtual std::unique_ptr<IInputIteratorLogic<T>> clone() const = 0;
    };

} // namespace common_NS
