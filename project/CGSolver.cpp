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
	// just follow the pesudocode of the algorithm to implement the actual CG algorithm
	std::vector<double> Ax = csr_mat_vec_product(val, row_ptr, col_idx, x);
	// r0 = b - A * u0
	std::vector<double> r = vec_add_with_coeff(b, Ax, -1);
	double r0_norm = norm(r);	// 2-norm of r0
	std::vector<double> p = r;	// p0 = r0
    std::size_t niter = 0;

    // for my implementation, just define the maximum number of iteration equal to the dimension of the matrix
	while (niter < row_ptr.size() - 1) {
		niter++;
		std::vector<double> Ap = csr_mat_vec_product(val, row_ptr, col_idx, p);
		double r_trans_r = vec_dot_product(r, r);
		// compute alpha = (rn.trans * rn)/(pn.trans * A * pn)
		double alpha = r_trans_r/vec_dot_product(p, Ap);
		// updtae x and r
		x = vec_add_with_coeff(x, p, alpha);
		r = vec_add_with_coeff(r, Ap, -1 * (alpha));
		double r_norm = norm(r);	// 2-norm of r(n+1)
		// termination condition
		if ((r_norm/r0_norm) < tol) {
			return int(niter);
        }
        // compute beta = (r(n+1).trans * r(n+1))/(rn.trans * rn)
		double beta = vec_dot_product(r, r)/r_trans_r;
		// update p
		p = vec_add_with_coeff(r, p, beta);
	}

	// otherwise the algorithm diverge
	return -1;
}