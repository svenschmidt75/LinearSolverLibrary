/*
 * Name  : VectorBinaryReader
 * Path  : 
 * Use   : Base class for reading vector data. Makes sense, because
           the client need not know how the vector data is stored
           internally.
 * Author: Sven Schmidt
 * Date  : 09/08/2012
 */
#pragma once

#include "IVectorReader.h"


namespace EntityReader_NS {

    class VectorBinaryReader : public IVectorReader {
    public:
        VectorBinaryReader(std::string const & filename);

        // FROM IVectorReader
        bool              read() const override;
        LinAlg_NS::Vector get() const override;

    private:
        std::string               filename_;
        mutable LinAlg_NS::Vector data_;
    };

} // namespace EntityReader_NS
