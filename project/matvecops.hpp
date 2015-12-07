#ifndef MATVECOPS_HPP
#define MATVECOPS_HPP

#include <vector>

/* Vector addition with scalar multiplication, returns a vector of (x + by) */
std::vector<double> vec_add_with_coeff(std::vector<double> &x, std::vector<double> &y, double b);

/* The dot product of  x transpose and y */
double vec_dot_product(std::vector<double> &x, std::vector<double> &y);

/* The 2-norm of a vector x */
double norm(std::vector<double> &x);

/* The sparse-matrix-vector multilication Ax with A in CSR format */
std::vector<double> csr_mat_vec_product(std::vector<double> &val,
										std::vector<int>    &row_ptr, 
             							std::vector<int>    &col_idx,
             							std::vector<double> &x);

#endif /* MATVECOPS_HPP */