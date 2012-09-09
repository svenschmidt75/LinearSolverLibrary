#include "FloridaSparseMatrixReaderLib/FloridaSparseMatrixReader.h"
#include "FloridaSparseMatrixReaderLib/SparseMatrixBuilder.h"

#include "LinAlg/SparseMatrix2D.h"

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


/* The following specializations are needed for serializing
 * in the xml file format. Not sure why...
 */
namespace boost {
    namespace serialization {

        template<>
            struct is_wrapper<LinAlg_NS::SparseMatrix2D> : public boost::mpl::true_ {
        };

        template<>
        struct is_wrapper<int> : public boost::mpl::true_ {
        };

    }
}



/* Usage: 1. Argument: Name of the file containing the sparse matrix data.
 * Matrices can be found here: "The University of Florida Sparse Matrix Collection",
 * http://www.cise.ufl.edu/research/sparse/matrices/
 * File format: Matrix Market Format (mtx)
 *
 * The matrix data will be serialized into a binary format.
 */

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Argument error.\nFilename of mtx file expected.\n" << std::endl;
        return 1;
    }

    FS::path filename(argv[1]);
    
    SparseMatrixBuilder builder;
    FloridaSparseMatrixReader reader(filename.string(), builder);
    bool success = reader.read();

    if (!success) {
        std::cerr << "Error converting file " << filename << std::endl;
        return 1;
    }

    SparseMatrixBuilder::result_t m = builder.result();


    // write serialized sparse matrix into same directory
    FS::path output_file = filename.parent_path() / (filename.stem().string() + ".ar");
    std::ofstream file(output_file.string(), std::ios_base::binary);
    BOOST_ASSERT_MSG(file.good(), "Error opening output file name");
//    boost::archive::text_oarchive oa(file); 
//    boost::archive::xml_oarchive oa(file); 
    boost::archive::binary_oarchive oa(file); 
    LinAlg_NS::SparseMatrix2D const & tmp = *m;
    oa << BOOST_SERIALIZATION_NVP(tmp);
    file.close();

	return 0;
}
