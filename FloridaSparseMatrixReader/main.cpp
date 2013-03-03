#include "pch.h"

#include "FloridaSparseMatrixReaderLib/FloridaSparseMatrixReader.h"
#include "FloridaSparseMatrixReaderLib/FloridaSparseMatrixBuilder.h"

#include "LinAlg/SparseMatrix2D.h"

using namespace EntityReader_NS;

namespace FS = boost::filesystem;


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
    
    FloridaSparseMatrixBuilder builder;
    FloridaSparseMatrixReader reader(filename.string(), builder);

    bool success = false;

    try {
        success = reader.read();
    }
    catch (std::exception const & ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }

    if (!success) {
        std::cerr << "Error converting file " << filename << std::endl;
        return 1;
    }

    FloridaSparseMatrixBuilder::result_t m = builder.result();


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
