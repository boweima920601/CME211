#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#include "matvecops.hpp"
#include "CGSolver.hpp"

void WriteSoln(std::vector<double> &x, std::string name) {
    std::ofstream f(name);
	std::size_t dim = x.size();
	// write the solution line by line
	if (f.is_open()) {
		for (std::size_t i = 0; i < dim; i++) {
			f << x[i] << std::endl;
		}

		f.close();
	}
	else {
		std::cerr << "ERROR: Unable to write solution file!" << std::endl;
		exit(1);
	}
}

int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol,
             std::string 		 soln_prefix)
{
	// just follow the pesudocode of the algorithm to implement the actual CG algorithm
	std::vector<double> Ax = csr_mat_vec_product(val, row_ptr, col_idx, x);
	// r0 = b - A * u0
	std::vector<double> r = vec_add_with_coeff(b, Ax, -1);
	double r0_norm = norm(r);	// 2-norm of r0
	std::vector<double> p = r;	// p0 = r0
    std::size_t niter = 0;

    // write the initial guess
    std::stringstream s;
    s << std::setfill('0') << std::setw(3) << 0;
    std::string filename = soln_prefix + s.str() + ".txt";
    WriteSoln(x, filename);

    // for my implementation, just define the maximum number of iteration equal to the dimension of the matrix
	while (niter < row_ptr.size() - 1) {
		niter++;
		std::vector<double> Ap = csr_mat_vec_product(val, row_ptr, col_idx, p);
        //std::cout << Ap[0] << std::endl;
		double r_trans_r = vec_dot_product(r, r);
        //std::cout << r_trans_r << std::endl;
		// compute alpha = (rn.trans * rn)/(pn.trans * A * pn)
		double alpha = r_trans_r/vec_dot_product(p, Ap);
        //std::cout << alpha << std::endl;
		// updtae x and r
		x = vec_add_with_coeff(x, p, alpha);
        //std::cout << x[0] << std::endl;
		r = vec_add_with_coeff(r, Ap, -1 * (alpha));
        //std::cout << r[0] << std::endl;
		double r_norm = norm(r);	// 2-norm of r(n+1)
		// termination condition
		if ((r_norm/r0_norm) < tol) {
			
            std::stringstream s;
            s << std::setfill('0') << std::setw(3) << niter;
            std::string filename = soln_prefix + s.str() + ".txt";
            WriteSoln(x, filename);
            
			return int(niter);
        }
        // compute beta = (r(n+1).trans * r(n+1))/(rn.trans * rn)
		double beta = vec_dot_product(r, r)/r_trans_r;
		// update p
		p = vec_add_with_coeff(r, p, beta);

		// write the solution every 10 iterations
		if (niter % 10 == 0) {
            std::stringstream s;
            s << std::setfill('0') << std::setw(3) << niter;
            std::string filename = soln_prefix + s.str() + ".txt";
            WriteSoln(x, filename);
		}
	}

	// otherwise the algorithm diverge
	return -1;
}