"""	print the pressure data of a particular airfoil in the desired format.
	
	This python script contains the actual implementation of the Airfoil class, which 
"""

import sys
import glob
import os
import math

class Airfoil:
	def __init__(self, inputdir):
		""" initiate the airfoil object from the input directory"""

		# check whether all the direcory provided contains all the required data files 
		if self.check_file_error(inputdir):
			sys.exit()

		# initiates class variables
		self.x = list()
		self.y = list()
		self.alphas = list()		
		self.pressure_coeff_list = dict()

		self.name = self.read_in_data(inputdir)


	def check_file_error(self, direc):
		""" check whether the provided directory has the required data files
			if not, return True and exit the program
		"""

		# check whether the user has provided an existing directory
		if not os.path.isdir(direc):
			print("{} is not an existing directory, please provide the correct airfoil directory.".format(direc))
			return True		
		
		# check whether the directory contains at least one alpha*.dat
		filenames = glob.glob(direc + 'alpha*.dat')
		if not filenames:
			print("Please provide at least one file of alpha*.dat as the pressure coefficients data.")
			return True

		# check whether the directory contains xy.dat
		xy = direc + 'xy.dat'
		if not os.path.isfile(xy):
			print("{} is not in the directory, please provide an xy.dat file as the geometry data of the airfoil.".format(xy))
			return True

		return False


	def read_in_data(self, inputdir):
		""" read all the data files provided in the given directory into our Airfoil object"""

		# create a list of xy coordinates
		#--functionality_1
		#--What if inputdir does not have a '/' at the end?! Should use os.path.join() and other modules, like you did in other methods!!!
		#--START
		f = open(inputdir + 'xy.dat', 'r')
		name = f.readline().strip()
		#--END
		# read in x, y coordinates
		for line in f:
			self.x.append(float(line.split()[0]))
			self.y.append(float(line.split()[1]))
		f.close()

		# read all the pressure coefficient data into a dictionary, which uses alpha as key and the Cp data as value
		alphas_list = glob.glob(inputdir + 'alpha*.dat')
		for alpha_name in alphas_list:
			#extract the alpha value out using substring
			alpha_val = float(alpha_name[alpha_name.find('alpha') + 5: alpha_name.find('.dat')])
			self.alphas.append(alpha_val)
			alpha = list()
			f = open(alpha_name)
			# read pass the "#Cp" line
			f.readline()

			for line in f:
				alpha.append(float(line.strip()))
			f.close()
			self.pressure_coeff_list[alpha_val] = alpha

		# return the name of the airfoil for latter use
		return name

	def compute_chord(self):
		""" compute the chord length using the difference of maximum x coordinates and minimum x coordinates
			an alternative way of computing chord length using Euclidean distance is given in comment
		"""
#--style_1
#--Remove commented-out code before submission!!!
#--START
		xmin = min(abs(x) for x in self.x)
#		xymin = (xmin, self.y[self.x.index(xmin)])
		xmax = max(abs(x) for x in self.x)
#		xymax = (xmax, self.y[self.x.index(xmax)])

#		length = math.sqrt((xymin[0] - xymax[0]) ** 2 + (xymin[1] - xymax[1]) ** 2)
		length = abs(xmax - xmin)
#--END
		return length


	def compute_lift_coefficient(self, pressure_list, alpha):
		""" compute the total lift coefficient of a aifoil with given alpha and returns it"""

		delta_cxs = list()
		delta_cys = list()

		# compute the chord length first
		chord = self.compute_chord()
		
		for i in range(len(self.x) - 1):
			dx = self.x[i] - self.x[i + 1]
			dy = self.y[i] - self.y[i + 1]
			dcx = pressure_list[i] * abs(dy) / chord
			dcy = pressure_list[i] * abs(dx) / chord
			# if (dx  == 0):
			#  	delta_cxs.append(0)
			#  	delta_cys.append(pressure_list[i])

			# deal with the sign problem of delta_cx and delta_cy
			# if dx > 0, then delta_cy < 0
			# if dy > 0, then delta_cx > 0
			# vice versa
			if (dx > 0):
				delta_cys.append((-1) * dcy)
				if (dy > 0):
					delta_cxs.append(dcx)
				elif (dy == 0):
					delta_cxs.append(0)
				else:
					delta_cxs.append((-1) * dcx)
			else:
				delta_cys.append(dcy)
				if (dy > 0):
					delta_cxs.append(dcx)
				elif (dy == 0):
					delta_cxs.append(0)
				else:
					delta_cxs.append((-1) * dcx)
#--design_2
#--Storing delta cx and delta cy in a list, ever growing the list. Inefficient and unnecessary.
#--Can just have some memory allocated for a float() and increment the it by the delta cx and cy!
#--What if the discretization of the wing was very fine? In the limit it could yield infinite datapoints. Blowing up the system....
#--START
		cx = sum(delta_cxs)
		cy = sum(delta_cys)
#--END
		ct = cy * math.cos(alpha * math.pi/180) - cx * math.sin(alpha * math.pi/180)

		return ct


	def compute_stag_pt(self, coeff):
		""" compute the stagnation point of the airfoil, then returns its x coordinate, y coordinate and its Cp value"""

		# find the Cp that is cloest to +1
		coeff_closest_to_one = min(coeff, key = lambda a:abs(a - 1))
		index = coeff.index(coeff_closest_to_one)
		x_avg = (self.x[index] + self.x[index + 1])/2
		y_avg = (self.y[index] + self.y[index + 1])/2

		return x_avg, y_avg, coeff_closest_to_one

	def __repr__(self):
		""" implement the representation of the airfoil object, formatted as required"""

		#--style_0
		#--Why don't you use the same column formatting for the headers??
		#--START
		string = "Test case: {}\n".format(self.name)
		string += "\nalpha    cl          stagnation pt      \n"
		string += "----- ------- --------------------------\n"
#--END
		# the required table is sorted by increasing order of alpha 
		alphas = sorted(self.alphas)
		for i in range(len(alphas)):

			ct = self.compute_lift_coefficient(self.pressure_coeff_list[alphas[i]], alphas[i])
			stag_x, stag_y, val = self.compute_stag_pt(self.pressure_coeff_list[alphas[i]])
			string += "{0:>5.2f} {1:>7.4f} ( {2:<6.4f}, {3:>7.4f}) {4:>7.4f}\n".format(alphas[i], ct, stag_x, stag_y, val)
		
		return string

#--writeup_1
#--Should've elaborated more on encapsulation and decomposition. In particular to OOP specific. What you mentioned is very general and is also applicable to functions...
#--END

#--functionality_6
#--We asked to raise 3 exceptions, there are 3 missing, instead you did sys.exit() which terminates the full stack wihout useful exception object
#--END		

#--style_2
#--Did not use git properly. not-meaningful git commit messages
#--END