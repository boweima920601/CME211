#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>

#include "heat.hpp"

int HeatEquation2D::Setup(std::string inputfile) {
    std::ifstream f(inputfile);
	SparseMatrix A;
	std::vector<double> b, x;
	double l, w, h, Tc, Th;
	int m, n, nrows, ncols, nx, ny;
	if (f.is_open()) {
		if (f >> l >> w >> h >> Tc >> Th) {
            m = (int)(l/h) + 1;
			n = (int)(w/h) + 1;
			nx = m - 1;
			ny = n - 2;
			nrows = nx * ny;
			ncols = nrows;
			A.Resize(nrows, ncols);

			for (int i = 0; i < nrows; i++) {
				if (i % nx == 0) {
						// points adjacent to the cold isothermal boundary.
						if (i < nx) {
							double Tx = (-1) * Tc * (exp(-10 * pow((i*h - l/2), 2)) - 2); 
							b.push_back(Tx);
                            // u(i, j)
                            A.AddEntry(i, i, 4);
                            // u(i - 1, j)
                            A.AddEntry(i, i + nx - 1, -1);
                            // u(i + 1, j)
                            A.AddEntry(i, i + 1, -1);
                            // u(i, j + 1)
                            A.AddEntry(i, i + nx, -1);
						}
						// points adjacent to the hot isothermal boundary.
						else if (i + nx >= nrows) {
							b.push_back(Th);
                            // u(i, j)
                            A.AddEntry(i, i, 4);
                            // u(i - 1, j)
                            A.AddEntry(i, i + nx - 1, -1);
                            // u(i + 1, j)
                            A.AddEntry(i, i + 1, -1);
                            // u(i, j - 1)
                            A.AddEntry(i, i - nx, -1);
						}
						// internal periodic nodes
						else {
							b.push_back(0);
                            // u(i, j)
                            A.AddEntry(i, i, 4);
                            // u(i - 1, j)
                            A.AddEntry(i, i + nx - 1, -1);
                            // u(i + 1, j)
                            A.AddEntry(i, i + 1, -1);
                            // u(i, j - 1)
                            A.AddEntry(i, i - nx, -1);
                            // u(i, j + 1)
                            A.AddEntry(i, i + nx, -1);
						}
				}
                // points adjacent to periodic boundary on the right
				else if ((i + 1) % nx == 0) {
					if (i < nx) {
						double Tx = (-1) * Tc * (exp(-10 * pow((i*h - l/2), 2)) - 2); 
						b.push_back(Tx);
                        // u(i, j)
                        A.AddEntry(i, i, 4);
                        // u(i - 1, j)
                        A.AddEntry(i, i - 1, -1);
                        // u(i + 1, j)
                        A.AddEntry(i, i - (nx - 1), -1);
                        // u(i, j + 1)
                        A.AddEntry(i, i + nx, -1);
					}
					// points adjacent to the hot isothermal boundary.
					else if (i + nx >= nrows) {
						b.push_back(Th);
                        // u(i, j)
                        A.AddEntry(i, i, 4);
                        // u(i - 1, j)
                        A.AddEntry(i, i - 1, -1);
                        // u(i + 1, j)
                        A.AddEntry(i, i - (nx - 1), -1);
                        // u(i, j - 1)
                        A.AddEntry(i, i - nx, -1);
					}
					// internal right periodic nodes
					else {
						b.push_back(0);
                        // u(i, j)
                        A.AddEntry(i, i, 4);
                        // u(i - 1, j)
                        A.AddEntry(i, i - 1, -1);
                        // u(i + 1, j)
                        A.AddEntry(i, i - (nx - 1), -1);
                        // u(i, j - 1)
                        A.AddEntry(i, i - nx, -1);
                        // u(i, j + 1)
                        A.AddEntry(i, i + nx, -1);
					}
				}
				else {
					if (i < nx) {
						double Tx = (-1) * Tc * (exp(-10 * pow((i*h - l/2), 2)) - 2); 
						b.push_back(Tx);
                        // u(i, j)
                        A.AddEntry(i, i, 4);
                        // u(i - 1, j)
                        A.AddEntry(i, i - 1, -1);
                        // u(i + 1, j)
                        A.AddEntry(i, i + 1, -1);
                        // u(i, j + 1)
                        A.AddEntry(i, i + nx, -1);
					}
					// points adjacent to the hot isothermal boundary.
					else if (i + nx >= nrows) {
						b.push_back(Th);
                        // u(i, j)
                        A.AddEntry(i, i, 4);
                        // u(i - 1, j)
                        A.AddEntry(i, i - 1, -1);
                        // u(i + 1, j)
                        A.AddEntry(i, i + 1, -1);
                        // u(i, j - 1)
                        A.AddEntry(i, i - nx, -1);
					}
					// internal nodes
					else {
						b.push_back(0);
                        // u(i, j)
                        A.AddEntry(i, i, 4);
                        // u(i - 1, j)
                        A.AddEntry(i, i - 1, -1);
                        // u(i + 1, j)
                        A.AddEntry(i, i + 1, -1);
                        // u(i, j - 1)
                        A.AddEntry(i, i - nx, -1);
                        // u(i, j + 1)
                        A.AddEntry(i, i + nx, -1);
					}
				}
				x.push_back(0);
			}
            
            A.ConvertToCSR();
			this->A = A;
            
			this->b = b;
			this->x = x;
            
            return 0;
		}

		else {
			std::cerr << "Input file has incorrect format." << std::endl;
			return 1;			
		}
	}
	else {
		std::cerr << "Could not open input file." << std::endl;
		return 1;
	}
}

int HeatEquation2D::Solve(std::string soln_prefix) {
	int CG_iter = this->A.CG(this->b, this->x, 1.e-5, soln_prefix);

	if (CG_iter >= 0) {
		std::cout << "SUCCESS: CG solver converged in "<< CG_iter << " iterations." << std::endl;
		return 0;
	}
	else {
		std::cout << "The CG algorithm does not converge." << std::endl;
		return 1;
	}
	return 1;
}