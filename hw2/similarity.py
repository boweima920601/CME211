import numpy as np
import sys
import time
from math import sqrt
import heapq

# Compute the cosine similarity of two movies using the instruction given.
def cosine_similarity(x, y, xbar, ybar):
	x_standardized = x - xbar
	y_standardized = y - ybar

	#--design_1
	#--This goes over the full numpy object again and count just to check wheter one or the other is zero or not. Think about it, that is very inefficient and is probably the reason why your code runs quite slow. Only 1 points deducted, but its importance is much higher! Think about this next time
	#--START
	all_same = (np.count_nonzero(x_standardized) ==0) or (np.count_nonzero(y_standardized) == 0)
	#--END
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
#--design_0
#--Main() func missing!, the global namespace is now cluttered with stuff!
#--END

#--functionality_2
#--No welcome message with number of lines read etc.. seems like screen freezes!
#--END

#--functionality_2
#--No correct warning for missing input arguments! It should do an exit with a correct warning message. Now it just returns None and continues
#--END

#--design_2
#--Reading data and processing it should all be in one or more functions. Your global namespace is really messy right now with all these data IO calls
#--START
movie_data, threshold, nmax = read_data(sys.argv)

# Get the number of users and number of movies.
user_list = list(set(movie_data[:, 0]))
movie_list = list(set(movie_data[:, 1]))
#--END

num_user = len(user_list)
num_movie = len(movie_list)
num_rating = len(movie_data[:, 0])

#--functionality_2
#--It is really inefficient to first load in all the data (and store it) and then copy it all to seperate datastructures. Should do that in one go! This will affect speed and memory usage. Bigger files may blow up the memory and throw weird errors...
#--START
movie_similarity = np.zeros((num_movie, num_movie))
movie_rating = np.zeros((num_user, num_movie))
movie_common_user_rating = {}
movie_rating_avg = np.zeros(num_movie)
#--END

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
#--functionality_1
#--Next time use a filestream or similar to continuously keep writing data to a file, that way you don;t need to keep all this stuff in-memory. In-memory is very slow since it does not allow effecieint usage of the cache since the cache is full always, and every item needs to be trerieved from memory. I know this is quite advacned and you dont need to know this, but you probavbly have an understanding of storing stuff in memory and that it is best to just write it to a file immediately! :)
#--START
f = open(sys.argv[2], 'w')

for i in range(num_movie):
	movie_id = int(movie_list[i])
	sim_list = list(movie_similarity[i])
	max_sim = max(sim_list)

#--END

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

#--functionality_15
#--Output is incorrect. It produces wrong highest similarity movie ids and coefficients!
#--END

#--functionality_15
#--Code runs more than 10 minutes (420 sec on corn). Could be much faster with more thought on implementation
#--END

#--style_1
#--Lack of git commits!
#--END