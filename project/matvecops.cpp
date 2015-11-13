#include <vector>
#include <math.h>

#include "matvecops.hpp"

/* Vector addition with scalar multiplication, returns a vector of (x + by) */

std::vector<double> vec_add_with_coeff(std::vector<double> &x, std::vector<double> &y, double b) {

	std::vector<double> sum;
	for (unsigned int i = 0; i < x.size(); i++) {
		sum.push_back(x[i] + b * y[i]);
	}

	return sum;
}

/* The dot product of  x transpose and y */

double vec_dot_product(std::vector<double> &x, std::vector<double> &y) {
	double sum = 0; 
	for (unsigned int i = 0; i < x.size(); i++) {
		sum += x[i] * y[i];
	}

	return sum;
}

/* The 2-norm of a vector x */

double norm(std::vector<double> &x) {
	return sqrt(vec_dot_product(x, x));
}

/* The sparse-matrix-vector multilication Ax with A in CSR format */

std::vector<double> csr_mat_vec_product(std::vector<double> &val,
										std::vector<int>    &row_ptr, 
             							std::vector<int>    &col_idx,
             							std::vector<double> &x)
{
	std::vector<double> vec;

    std::size_t dim = row_ptr.size() - 1;
    // iterate through row using the advantage of CSR format
	for (unsigned int i = 0; i < dim; i ++) {
		double k = 0;
		// compute the partial sum of A[i] * x
		for (int j = int(row_ptr[i]); j < int(row_ptr[i + 1]); j++) {
			k += val[j] * x[col_idx[j]];
		}
		vec.push_back(k);
	}

	return vec;
}