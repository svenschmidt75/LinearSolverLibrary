#include "pch.h"

#include "SparseMatrixReaderCreator.h"

#include "ISparseMatrixReader.h"
#include "SparseMatrixBinaryReader.h"

#include "common/reporting.h"


using namespace EntityReader_NS;

namespace FS = boost::filesystem;


ISparseMatrixReader::Ptr
SparseMatrixReaderCreator::create(std::string const & filename) {
    // create reader depending on the file extension

    if (!FS::exists(filename)) {
        boost::format format = boost::format("SparseMatrixReaderCreator::create: File %1% not found!\n") % filename;
        common_NS::reporting::error(format.str());
        return nullptr;
    }
    
    std::string ext = FS::extension(filename);

    if (ext == ".ar") {
        // create binary reader
        ISparseMatrixReader::Ptr binary_reader(new SparseMatrixBinaryReader(filename));
        return binary_reader;
    }

    return nullptr;
}
