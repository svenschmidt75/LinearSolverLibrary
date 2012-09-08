/*
 * Name  : SparseMatrixReaderCreator
 * Path  : 
 * Use   : Creates a sparse matrix reader depending on the
           type of the serialized data.
 * Author: Sven Schmidt
 * Date  : 09/08/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ISparseMatrixReader.h"

#include <string>


namespace EntityReader_NS {

    class FLORIDASPARSEMATRIXREADERLIB_DECL_SYMBOLS SparseMatrixReaderCreator {
    public:
        static ISparseMatrixReader::Ptr create(std::string const & filename);

    };

} // namespace EntityReader_NS
