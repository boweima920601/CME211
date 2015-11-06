import sys
import numpy as np

# if the user does not provide a valid input format, print usage message and exit
if (len(sys.argv) < 3):

	print('Usage:')
	print('  python {} [maze file] [solution file]'.format(sys.argv[0]))
	exit()

# read in the wall data into a numpy array
try:
	walls = np.loadtxt(sys.argv[1], dtype = int)
except IOError:
	print("Please provide a valid maze file.")
	exit()
row = walls[0, 0]
col = walls[0, 1]

# set up the maze array
maze = np.zeros((row, col))
for i in range(1, len(walls[:, 0])):
	maze[walls[i, 0], walls[i, 1]] = 1
try:
	sol = np.loadtxt(sys.argv[2], dtype = int)
except IOError:
	print("Please provide a valid solution file.")
	exit()

# if the entrance is not valid, print invalid warning and exit
if (sol[0, 0] != 0 or maze[sol[0, 0], sol[0, 1]] != 0):
	print('Solution is invalid!')
	exit()
for i in range(1, len(sol[:, 0])):
	# if making a move with inconsecutive displacement or move into a wall, print invalid warning and exit
	if ((abs(sol[i, 0] - sol[i - 1, 0]) + abs(sol[i, 1] - sol[i - 1, 1])) > 1 or (maze[sol[i, 0], sol[i, 1]] != 0)):
		print('Solution is invalid!')
		exit()

# if the exit is not at the last row or the exit is a wall, print invalid and exit
exit = sol[len(sol[:, 0]) - 1]
if (exit[0] != (row - 1) or maze[exit[0], exit[1]] != 0):
	print('Solution is invalid!')
	exit()

# otherwise we have got a valid solution
print('Solution is valid!')