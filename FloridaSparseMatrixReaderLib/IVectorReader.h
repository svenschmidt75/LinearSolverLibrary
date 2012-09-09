/*
 * Name  : IVectorReader
 * Path  : 
 * Use   : Base class for reading vector data. Makes sense, because
           the client need not know how the vector data is stored
           internally.
 * Author: Sven Schmidt
 * Date  : 09/09/2012
 */
#pragma once

#include <memory>


namespace LinAlg_NS {
    class Vector;
}

namespace EntityReader_NS {

    class IVectorReader {
    public:
        typedef std::unique_ptr<IVectorReader> Ptr;

    public:
        virtual ~IVectorReader() {}

        virtual bool                    read() const = 0;
        virtual LinAlg_NS::Vector const get() const = 0;
    };

} // namespace EntityReader_NS
