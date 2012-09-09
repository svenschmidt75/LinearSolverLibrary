#include "VectorReaderCreator.h"

#include "IVectorReader.h"
#include "VectorBinaryReader.h"

#include "common/reporting.h"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>


using namespace EntityReader_NS;

namespace FS = boost::filesystem;


IVectorReader::Ptr
VectorReaderCreator::create(std::string const & filename) {
    // create reader depending on the file extension

    if (!FS::exists(filename)) {
        boost::format format = boost::format("VectorReaderCreator::create: File %1% not found!\n") % filename;
        common_NS::reporting::error(format.str());
        return nullptr;
    }

    std::string ext = FS::extension(filename);

    if (ext == ".ar") {
        // create binary reader
        IVectorReader::Ptr binary_reader(new VectorBinaryReader(filename));
        return binary_reader;
    }

    return nullptr;
}
