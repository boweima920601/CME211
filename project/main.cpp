#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "CGSolver.hpp"
#include "COO2CSR.hpp"

int main(int argc, char* argv[]) {
	
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
		//
		int m, n;
		f >> m >> n;

		if (m != n) {
			std::cerr << "ERROR: Matrix is not square" << std::endl;
			return 1;
		}

		double v;
		int i, j;
		while(f >> i >> j >> v) {
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

	COO2CSR(val, i_idx, j_idx);

	std::vector<double> x;
	std::vector<double> b;
    std::size_t dim = i_idx.size() - 1;

	for (unsigned int i = 0; i < dim; i++) {
		x.push_back(1.);
		b.push_back(0.);
	}

    int niter = CGSolver(val, i_idx, j_idx, b, x, 1.e-5);
    
    if (niter > 0) {
        std::ofstream f_out;
        f_out.open(argv[2]);
        if (f_out.is_open()) {
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