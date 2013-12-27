/*
 * Name  : FloridaVectorReader
 * Path  : 
 * Use   : Reads a vector in the format specified by the
           "The University of Florida Sparse Matrix Collection"
 * Author: Sven Schmidt
 * Date  : 09/09/2012
 */
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace LinAlg_NS {
    class Vector;
}


namespace EntityReader_NS {

    template<typename T>
    class IVectorBuilder;

    class FLORIDASPARSEMATRIXREADERLIB_DECL_SYMBOLS FloridaVectorReader {
    public:
        FloridaVectorReader(std::string const & filename, IVectorBuilder<LinAlg_NS::Vector> & builder);
        FloridaVectorReader(FloridaVectorReader const &) = delete;
        FloridaVectorReader & operator=(FloridaVectorReader const &) = delete;

        bool read() const;

    private:
        std::string                         filename_;
        IVectorBuilder<LinAlg_NS::Vector> & builder_;
    };

} // namespace EntityReader_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
