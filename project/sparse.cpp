#include <string>
#include <vector>

#include "sparse.hpp"
#include "COO2CSR.hpp"
#include "matvecops.hpp"
#include "CGSolver.hpp"

void SparseMatrix::Resize(int nrows, int ncols) {
	this->nrows = nrows;
	this->ncols = ncols;
}

void SparseMatrix::AddEntry(int i, int j, double val) {
	this->i_idx.push_back(i);
	this->j_idx.push_back(j);
	this->a.push_back(val);
}

void SparseMatrix::ConvertToCSR() {
	COO2CSR(this->a, this->i_idx, this->j_idx);
}

std::vector<double> SparseMatrix::MulVec(std::vector<double> &vec) {
	return csr_mat_vec_product(this->a, this->i_idx, this->j_idx, vec);
}

int SparseMatrix::CG(std::vector<double> &b, std::vector<double> &x, double tol, std::string soln_prefix) {
	return CGSolver(this->a, this->i_idx, this->j_idx, b, x, tol, soln_prefix);
}