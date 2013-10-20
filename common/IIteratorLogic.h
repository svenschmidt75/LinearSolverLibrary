/*
* Name  : IIteratorLogic
* Path  : 
* Use   : Base class that implements the logic for iterators.
* Author: Sven Schmidt
* Date  : 10/20/2013
*/
#pragma once

#include <memory>


namespace common_NS {

    template<typename T>
    class IIteratorLogic {
    public:
        virtual bool isValid() const = 0;
        virtual void next() = 0;
        virtual T    get() const = 0;
        virtual std::unique_ptr<IIteratorLogic<T>> end() const = 0;
        virtual bool equalTo(IIteratorLogic const & in) const = 0;
    };

} // common_NS
