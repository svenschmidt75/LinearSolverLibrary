/*
 * Name  : VectorReaderCreator
 * Path  : 
 * Use   : Creates a vector data reader depending on the
           type of the serialized data.
 * Author: Sven Schmidt
 * Date  : 09/09/2012
 */
#pragma once

#include "DeclSpec.h"

#include "IVectorReader.h"

#include <string>


namespace EntityReader_NS {

    class FLORIDASPARSEMATRIXREADERLIB_DECL_SYMBOLS VectorReaderCreator {
    public:
        static IVectorReader::Ptr create(std::string const & filename);
    };

} // namespace EntityReader_NS
