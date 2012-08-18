/*
 * Name  : FloridaSparseMatrixReader
 * Path  : 
 * Use   : Reads a matrix in the format specified by the
           "The University of Florida Sparse Matrix Collection"
 * Author: Sven Schmidt
 * Date  : 08/12/2012
 */
#pragma once

#include "DeclSpec.h"

#include <boost/noncopyable.hpp>

#include <string>


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace EntityReader_NS {

    class IMatrixBuilder;

    class FLORIDASPARSEMATRIXREADER_DECL_SYMBOLS FloridaSparseMatrixReader : public boost::noncopyable {
    public:
        FloridaSparseMatrixReader(std::string const & filename, IMatrixBuilder & builder);

        bool read() const;

    private:
        std::string      filename_;
        IMatrixBuilder & builder_;
    };

} // namespace EntityReader_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
