import sys
import numpy as np
import warnings
import scipy.sparse.linalg as spla
import scipy.sparse as sp
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt


class Truss:
	def __init__(self, joint, beam, output = None):
		"""
		The constructor of the Truss class.
		Class variables are read and stored.
		"""

		# try to load the data into our object
		# if IO is failed, raise an error message
		try:
			self.joints = np.loadtxt(joint)
			self.beams = np.loadtxt(beam)
		except IOError:
			print('Please provide valid joints and beams data file paths.')
			exit()

		# use a dict to store the joint number with it coordinates
		self.coords = {}
		for i in range(len(self.joints[:, 0])):
			self.coords[self.joints[i, 0]] = [self.joints[i, 1], self.joints[i, 2]]

		# plot the truss structure if desired
		if (output):
			self.PlotGeometry(output)

	def PlotGeometry(self, output):
		"""
		Plot the structure of the truss.
		"""

		plt.figure()
		for k in range(len(self.beams[:, 0])):
			# record the two ends of a particular beam
			one_end = self.beams[k, 1]
			other_end = self.beams[k, 2]

			# draw a line between the two joints to represent the beam
			plt.plot([self.coords[one_end][0], self.coords[other_end][0]], [self.coords[one_end][1], self.coords[other_end][1]], color = 'b')

		# make the plot look nicer
		plt.axis('equal')
		# save the plot as the default .png file if not specified
		plt.savefig(output)

	def ComputeStaticEquilibrium(self):
		"""
		This function implements the real computation of the static equilbrium and returns the solution.
		The computation is implemented using sparse matrix computation.
		The sparse matrix A would be of the COO format, and converted to CSR format when solving.
		The right hand side b is of the format[-F1x, -F1y, -F2x, -F2y, ... , -Fnx, -Fny]
		"""

		# record the number of equations and unknowns to determine whether the system is over/under-determined
		no_joints = len(self.coords)
		no_beams = len(self.beams[:, 0])
		no_fixed_points = len(filter(None, self.joints[:, 5]))

		# raise a RuntimeError if the system is over/under-determined
		if not(2 * no_joints == no_beams + 2 * no_fixed_points):
			raise RuntimeError, "Truss geometry not suitable for static equilbrium analysis"
			exit()

		# data structures to store the COO format of the matrix
		data = []
		row = []
		col = []
		fixed_points = []
		b = np.zeros(2 * no_joints)
		# store the right hand side as b
		for i in range(no_joints):
			b[2 * i: 2 * (i + 1)] = -1 * self.joints[i, 3: 5]

			# record the joint number of the fixed points
			if self.joints[i, 5] != 0:
				fixed_points.append(self.joints[i, 0])


		# construct the coeeficient sparse matrix A using the COO format(data, (row, col))
		for i in range(no_beams):
			beam = self.beams[i, 1: 3]
			# compute the coeeficients of the system using trigonometry of each beam
			for j in range(2):
				dx = self.coords[beam[j]][0] - self.coords[beam[1 - j]][0]
				dy = self.coords[beam[j]][1] - self.coords[beam[1 - j]][1]
				angle = np.arctan2(dy, dx)
				s = np.sin(angle)
				c = np.cos(angle)

				# the index of the corresponding joint
				index = beam[j] - 1

				# Fx = F * cos(angle)
				data.append(c)
				row.append(2 * index)
				col.append(i)
				# Fy = F * sin(angle)
				data.append(s)
				row.append(2 * index + 1)
				col.append(i)

		# store the coefficients of the reaction forces due to the fixed supports
		for i in range(len(fixed_points)):
			index = fixed_points[i] - 1
			for j in range(2):
				data.append(1)
				row.append(2 * index + j)
				col.append(no_beams + 2 * i + j)

		# form the sparse matrix A using COO format
		A = sp.coo_matrix((data, (row, col)), shape = (2 * no_joints, 2 * no_joints))

		# test whether the linear system is singular before output the results
		# if the system is singular, raise an exception and print an useful message
		warnings.filterwarnings('error')
		warnings.filterwarnings('ignore', category = np.VisibleDeprecationWarning)
		warnings.filterwarnings('ignore', category = DeprecationWarning)
		try:
			x = spla.spsolve(A.tocsr(), b)
		except Exception:
			# raise a RuntimeError if the system is singular
			raise RuntimeError, "Cannot solve the linear system, unstable truss?"
			exit()

		return x

	def __repr__(self):
		"""
		This function implements the actual string representation of the Truss object, in the required format
		"""

		x = self.ComputeStaticEquilibrium()
		str_repr = "{0}{1: >9}\n".format("Beam".center(8), "Force")
		str_repr += "-----------------\n"
		for i in range(len(self.beams)):
			str_repr += "{0: >6}{1: >11.3f}\n".format(i + 1, x[i])

		return str_repr