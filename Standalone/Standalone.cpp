#include "pch.h"


// warning C4505: 'LinAlg_NS::operator *' : unreferenced local function has been removed
#pragma warning(disable:4505)
#pragma warning(default:4505)

using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;



int main(int /*argc*/, char* /*argv[]*/)
{
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\bcsstk05\\bcsstk05.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    BOOST_ASSERT_MSG(sm_reader->read(), "error reading sparse matrix data");

    SparseMatrix2D const m = sm_reader->get();
    BOOST_ASSERT_MSG(153ull == m.cols(), "error in number of columns");

    BOOST_ASSERT_MSG(LinAlg_NS::helper::isSymmsteric(m), "Matrix not symmetric");

    // m is NOT diagonally dominant
    BOOST_ASSERT_MSG(!SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m), "matrix should not be diagonally dominant");


    // create solution vector x
    Vector x_ref(153);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(153);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    int iterations;
    double tol;
    std::tie(success, x, iterations) = SparseLinearSolver::sparseSOR(m, b, 1.234, 200000);

    BOOST_ASSERT_MSG(success, "SOR failed to solve linear system");

    // compare vectors
//    BOOST_ASSERT_MSG(SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10), "mismatch in CG solver result");
    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m, LinAlg_NS::helper::transpose(m), b, 10000);
    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m, LinAlg_NS::helper::transpose(m), b, 10000);

    // needs 17687 iterations
    std::cout << "Iterations: " << iterations << std::endl;

	return 0;
}
