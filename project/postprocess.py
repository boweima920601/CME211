import sys
import math
import numpy as np
import matplotlib.pyplot as plt

def main(argv):
	# check usage message input
	if (len(argv) < 3):
		print('Usage:')
		print('  python {} [input file] [solution files]'.format(sys.argv[0]))
		exit()

	input_file = argv[1]
	soln_file = argv[2]

	print("Input file processed: {}".format(input_file))

	# read in the basic geometry information of the pipe
	data = list()
	f = open(input_file, 'r')
	for line in f:
		for x in line.split():
			data.append(float(x))
	f.close()

	soln = np.loadtxt(soln_file)

	[l, w, h, Tc, Th] = data
	# dimension of the pipe
	m = int(l/h) + 1
	n = int(w/h) + 1

	pipe = np.zeros((n, m), dtype = np.float64)

	# build the pipe by adding boundary points into the pipe
	# hot isothermal boundary
	pipe[0] = Th
	pipe[1 : n - 1, 0 : m - 1] = soln.reshape(n - 2, m - 1)
	pipe[1 : n - 1, m - 1] = pipe[1 : n - 1, 0]
	# cold isothermal boundary
	pipe[n - 1] = (-1) * Tc * (np.exp(-10 * np.power(np.array(range(m)) * h - l/2, 2)) - 2)

	# compute the mean temperature
	mean_temp = np.mean(pipe)
    
	print("Mean Temperature: {}".format(np.around(mean_temp, decimals = 5)))

	# Now let us plot the thermal plot
	plt.figure()
	plt.axis('equal')
	x = np.arange(0, l + h, h)
	y = np.arange(0, w + h, h)
	X, Y = np.meshgrid(x, y)
	# Use pcolormesh to guarantee faster execution
	plt.pcolormesh(X, Y, pipe)
	plt.colorbar()

	# Code for the interpolation to plot the isoline
	plt.contour(X, Y, pipe, [mean_temp], color = 'black', linestyle = '-')
	plt.savefig('figure.jpg')
	plt.show()
	
if __name__ == "__main__":
    main(sys.argv)