reffile = "reference3.txt"
readsfile = "reads3.txt"
alignfile = "alignments3.txt"

import time

# Read the dataset of the reference file.
f = open(reffile, 'r')
ref = f.read()
f.close()

# Read the given reads.
f = open(readsfile, 'r')
reads = f.readlines()
f.close()


f = open(alignfile, 'w')
reflength = len(ref)
nreads = len(reads)
aligns_0 = 0
aligns_1 = 0
aligns_2 = 0

# Before start, record the current time stamp and align the reads.
start = time.time()
for read in reads:
	first_pos = ref.find(read.rstrip('\n'))
	second_pos = 0
	
	if (first_pos < 0):
		aligns_0 += 1
		f.write("{} {}\n".format(read.rstrip('\n'), first_pos))

	elif (first_pos < reflength/2):
		aligns_1 += 1
		f.write("{} {}\n".format(read.rstrip('\n'), first_pos))
	
	# Find the addtional second alignment in the last part of the reference.
	else:
		aligns_2 += 1
		second_pos = ref.find(read.rstrip('\n'), (reflength * 3/4))
		f.write("{} {} {}\n".format(read.rstrip('\n'), first_pos, second_pos))

f.close()
end = time.time()
time_used = end - start

print("reference length: {}".format(reflength))
print("number reads: {}".format(nreads))
print("aligns 0: {}".format(float(aligns_0)/ nreads))
print("aligns 1: {}".format(float(aligns_1)/ nreads))
print("aligns 2: {}".format(float(aligns_2)/ nreads))
print("elapsed time: {}".format(time_used))