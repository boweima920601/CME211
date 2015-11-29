#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "CGSolver.hpp"
#include "COO2CSR.hpp"

int main(int argc, char* argv[]) {
	//	if the user does not provide correct command line argument, print an usage message and return
    if (argc < 3) {
		std::cerr << "Usage:" << std::endl;
		std::cerr << " " << argv[0] << "<COO-format matrix file> <solution file>" << std::endl;
		return 1;
    }

	std::vector<double> val;
	std::vector<int> i_idx;
	std::vector<int> j_idx;

	std::ifstream f;
	f.open(argv[1]);
	if (f.is_open()) {
		// check if the matrix is square, if not, print an error message and return
		int m, n;
		f >> m >> n;
		if (m != n) {
			std::cerr << "ERROR: Matrix is not square" << std::endl;
			return 1;
		}

		double v;
		int i, j;
		while(f >> i >> j >> v) {
			// read in the matrix using COO format
			val.push_back(v);
			i_idx.push_back(i);
			j_idx.push_back(j);
		}
        f.close();
	}

	else {
		std::cerr << "ERROR: Failed to open file" << std::endl;
		return 1;
	}

	// convert the COO matrix into CSR matrix
	COO2CSR(val, i_idx, j_idx);

	std::vector<double> x;
	std::vector<double> b;
    std::size_t dim = i_idx.size() - 1;

	for (unsigned int i = 0; i < dim; i++) {
    	// initial guess of x as all one's
		x.push_back(1.);
		b.push_back(0.);
	}

	// call the CG algorithm and record the number of iterations until convergence
    int niter = CGSolver(val, i_idx, j_idx, b, x, 1.e-5);
    
    // write solution to file if the algorithm converges
    if (niter > 0) {
        std::ofstream f_out;
        f_out.open(argv[2]);
        if (f_out.is_open()) {
        	// set to 4 decimal places with scientfic notation
            std::cout.setf(std::ios::scientific, std::ios::floatfield);
            std::cout.precision(4);

            for (unsigned int i = 0; i < dim; i++) {
                f_out << x[i] << std::endl;
            }
            f_out.close();
        }
        std::cout << "CG solver converged in " << niter << " iterations." << std::endl;
    }
    
    else {
        std::cout << "CG solver diverged." << std::endl;
    }
	return 0;
}


//--functionality_0
//--Good progress!!!
//--END