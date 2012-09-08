#include "SparseMatrixReaderCreator.h"

#include "ISparseMatrixReader.h"

#include <boost/filesystem.hpp>

using namespace EntityReader_NS;

namespace FS = boost::filesystem;


ISparseMatrixReader::Ptr
SparseMatrixReaderCreator::create(std::string const & filename) {
    // create reader depending on the file extension
    // throws runtime_error if unknown format

    if (!FS::exists(filename)) {
    }
    

// 
//     assert( !exists( "foo" ) );  // (2)
// 
// 
//     FS::path filename("\\Develop\\SparseMatrixData\\sts4098\\sts4098.ar");
//     SparseMatrix2D m;
// 
//     SparseMatrixReader & sm_reader = SparseMatrixReader::create(filename);
//     bool success = sm_reader.read();
//     m = sm_reader.get();
// 
// 
// 
//     std::ifstream file(filename.string(), std::ios_base::binary);
//     BOOST_ASSERT_MSG(file.good(), "Error opening output file name");
//     boost::archive::binary_iarchive oa(file); 
//     oa >> BOOST_SERIALIZATION_NVP(m);
//     file.close();


}
