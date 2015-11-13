#ifndef MATVECOPS_HPP
#define MATVECOPS_HPP

#include <vector>

std::vector<double> vec_add_with_coeff(std::vector<double> &x, std::vector<double> &y, double b);

double vec_dot_product(std::vector<double> &x, std::vector<double> &y);

double norm(std::vector<double> &x);

std::vector<double> csr_mat_vec_product(std::vector<double> &val,
										std::vector<int>    &row_ptr, 
             							std::vector<int>    &col_idx,
             							std::vector<double> &x);

#endif /* MATVECOPS_HPP */