#include <vector>

#include "matvecops.hpp"
#include "CGSolver.hpp"


int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr, 
             std::vector<int>    &col_idx,
             std::vector<double> &b, 
             std::vector<double> &x,
             double              tol)
{

	std::vector<double> Ax = csr_mat_vec_product(val, row_ptr, col_idx, x);
	std::vector<double> r = vec_add_with_coeff(b, Ax, -1);
	double r0_norm = norm(r);
	std::vector<double> p = r;
    std::size_t niter = 0;

	while (niter < row_ptr.size() - 1) {
		niter++;
		std::vector<double> Ap = csr_mat_vec_product(val, row_ptr, col_idx, p);
		double r_trans_r = vec_dot_product(r, r);
		double alpha = r_trans_r/vec_dot_product(p, Ap);
		x = vec_add_with_coeff(x, p, alpha);
		r = vec_add_with_coeff(r, Ap, -1 * (alpha));
		double r_norm = norm(r);
        if ((r_norm/r0_norm) < tol) {
			return int(niter);
        }
		double beta = vec_dot_product(r, r)/r_trans_r;
		p = vec_add_with_coeff(r, p, beta);
	}

	return -1;
}