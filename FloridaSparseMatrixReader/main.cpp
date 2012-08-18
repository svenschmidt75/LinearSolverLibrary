#include "FloridaSparseMatrixReaderLib/FloridaSparseMatrixReader.h"
#include "FloridaSparseMatrixReaderLib/SparseMatrixBuilder.h"


using namespace EntityReader_NS;


int main(int /*argc*/, char* /*argv[]*/)
{
    SparseMatrixBuilder builder;
    std::string filename = "\\Develop\\SparseMatrixData\\sts4098\\sts4098.mtx";
    FloridaSparseMatrixReader reader(filename, builder);
    bool success = reader.read();
    success = false;


	return 0;
}
