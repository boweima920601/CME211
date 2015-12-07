import numpy as np
import sys
import time
from math import sqrt
import heapq

# Compute the cosine similarity of two movies using the instruction given.
def cosine_similarity(x, y, xbar, ybar):
	x_standardized = x - xbar
	y_standardized = y - ybar

	all_same = (np.count_nonzero(x_standardized) ==0) or (np.count_nonzero(y_standardized) == 0)

	if all_same:
		return 0
	else:
		nominator = sum(x_standardized * y_standardized)
		denominator = sqrt(sum(x_standardized ** 2) * sum(y_standardized ** 2))

		return nominator/denominator

	return 0

# Read the command line argument and 
def read_data(arg):
	nmax = 1
	threshold = 5
	num_arg = len(arg)
	if (num_arg == 1):
		print("Please enter the name of the data and re-run the program again")
		return None, None, None 
	elif (num_arg > 3):
		threshold = int(arg[3])

	if (num_arg > 4):
		nmax = int(arg[4])

	data = np.loadtxt(arg[1], usecols = range(0, 3))
	return data, threshold, nmax

# Record the common rating list of two movies using a dict, which key is a tuple of the two movies 
# and value is the rating lists from the common users of the two movies.
def record_common_rating(rating, movie_map):
	m = len(rating)
	n = len(rating[0])

	for i in range(n):
		for j in range(i + 1, n):
			common_list_i = []
			common_list_j = []
			for k in range(m):
					if (rating[k, i] > 0 and rating[k, j] >0):
						common_list_i.append(rating[k, i])
						common_list_j.append(rating[k, j])
			movie_map[(i, j)] = (common_list_i, common_list_j, len(common_list_i))

	return movie_map

# Compute the average ratings from each movie.
def compute_rating_means(rating, avg):
	for i in range(len(rating[0])):
		avg[i] = np.mean(rating[:, i])
	return avg

# Compute the similarity matrix for each movie pairs.
def compute_similarity_matrix(common_dict, thres, avg, matrix):
	for pair, rating in common_dict.iteritems():
		i = pair[0]
		j = pair[1]
		if (len(rating[0]) >= thres):
			matrix[i, j] = matrix[j, i] = cosine_similarity(rating[0], rating[1], avg[i], avg[j])

	return matrix

####################################################################################################

movie_data, threshold, nmax = read_data(sys.argv)

# Get the number of users and number of movies.
user_list = list(set(movie_data[:, 0]))
movie_list = list(set(movie_data[:, 1]))

num_user = len(user_list)
num_movie = len(movie_list)
num_rating = len(movie_data[:, 0])

movie_similarity = np.zeros((num_movie, num_movie))
movie_rating = np.zeros((num_user, num_movie))
movie_common_user_rating = {}
movie_rating_avg = np.zeros(num_movie)

start = time.time()

# Record the ratings into an array of size(number of users, number of movies)
for i in range(num_rating):
	movie_rating[user_list.index(movie_data[i, 0]), movie_list.index(movie_data[i, 1])] = movie_data[i, 2]

movie_common_user_rating = record_common_rating(movie_rating, movie_common_user_rating)

movie_rating_avg = compute_rating_means(movie_rating, movie_rating_avg)

movie_similarity = compute_similarity_matrix(movie_common_user_rating, threshold, movie_rating_avg, movie_similarity)

end = time.time()

print("Input MovieLens file: {}".format(sys.argv[1]))
print("Output file for similarity data: {}".format(sys.argv[2]))
print("Minimum number of common users: {}".format(threshold))
print("Read {} lines with total of {} movies and {} users".format(num_rating, num_movie, num_user))
print("Computed simiarities in {} seconds".format(end - start))

# Finally, process the output file in the demand format.
f = open(sys.argv[2], 'w')

for i in range(num_movie):
	movie_id = int(movie_list[i])
	sim_list = list(movie_similarity[i])
	max_sim = max(sim_list)
	
	if (max_sim > 0):
		str_list = str()
		index = sim_list.index(max_sim)
		max_movie_id = int(movie_list[index])
		temp = (i, index)
		str_list = "({},%.2f,{})".format(max_movie_id, movie_common_user_rating[tuple(sorted(temp))][2])%(max_sim)


		f.write("{} {}\n".format(movie_id, str_list))
	else:
		f.write("{}\n".format(movie_id))
f.close()
