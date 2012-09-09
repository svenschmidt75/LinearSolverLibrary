#include "FloridaSparseMatrixReaderLib/FloridaVectorReader.h"
#include "FloridaSparseMatrixReaderLib/FloridaVectorBuilder.h"

#include "LinAlg/Vector.h"

#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/assert.hpp>

#include <iostream>
#include <cassert>


using namespace EntityReader_NS;

namespace FS = boost::filesystem;


/* Usage: 1. Argument: Name of the file containing the vector data.
 * Matrices can be found here: "The University of Florida Sparse Matrix Collection",
 * http://www.cise.ufl.edu/research/sparse/matrices/
 * File format: Matrix Market Format (mtx)
 *
 * The vector data will be serialized into a binary format.
 */

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Argument error.\nFilename of mtx file expected.\n" << std::endl;
        return 1;
    }

    FS::path filename(argv[1]);
    
    FloridaVectorBuilder builder;
    FloridaVectorReader reader(filename.string(), builder);
    bool success = reader.read();

    if (!success) {
        std::cerr << "Error converting file " << filename << std::endl;
        return 1;
    }

    FloridaVectorBuilder::result_t m = builder.result();

    // write serialized sparse matrix into same directory
    FS::path output_file = filename.parent_path() / (filename.stem().string() + ".ar");
    std::ofstream file(output_file.string(), std::ios_base::binary);
    BOOST_ASSERT_MSG(file.good(), "Error opening output file name");
//    boost::archive::text_oarchive oa(file); 
//    boost::archive::xml_oarchive oa(file); 
    boost::archive::binary_oarchive oa(file); 
    LinAlg_NS::Vector const & tmp = *m;
    oa << BOOST_SERIALIZATION_NVP(tmp);
    file.close();

    return 0;
}
