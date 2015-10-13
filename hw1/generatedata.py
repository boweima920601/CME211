reffile = "reference3.txt"
readsfile = "reads3.txt"

reflength = 100000
nreads = 60000
readlength = 50

import numpy as np
import random

# Create a dictionary for the gene sequence.
gene = {0: 'A', 1: 'C', 2: 'T', 3: 'G'}

ref = ""
halflen = reflength/2

# Create a randomlized gene reference of the given length.
#--functionality_2
#--Precarious index calculation. What if reflength = 998? Then end reference string length is 997! Be more rigorous
#--Good practice is also to do a sanity check after you create the ref, to see if length matches reflength
#--START
for i in range((reflength * 3/4)):
	ref += gene[random.randint(0, 3)]
ref += ref[halflen:]
#--END

# Write the reference into the required file.
f = open(reffile, 'w')
f.write(ref)
f.close()

# Create the reads corresponding to 0, 1, or 2 alignments.
f = open(readsfile, 'w')
aligns_0 = 0
aligns_1 = 0
aligns_2 = 0
for i in range(nreads):
	loc = random.random()
	
	# In this 75% quantile we will have reads that only aligns once.
	if loc <= 0.75:
		aligns_1 += 1
		pos = random.randint(0, halflen - 1)
		f.write(ref[pos: pos + readlength] + '\n')

	# 10% of the reads will align twice.
	elif loc <= 0.85:
		aligns_2 += 1
		pos = random.randint((reflength * 3/4), reflength - readlength - 1)
		f.write(ref[pos: pos + readlength] + '\n')

	# 15% rest of the reads will be guarnteed to not align.
	else:
		aligns_0 += 1
		#--style_0
		#--Just for future considerations: don't need a flag, can just break out with 'breal' ;)
		#--START
		flag = True
		while flag:
		#--END
			read_0 = ''
			for i in range(readlength):
				read_0 += gene[random.randint(0, 3)]
			if (ref.find(read_0) == -1):
				flag = False
				f.write(read_0 + '\n')

f.close()

print("reference length: {}".format(reflength))
print("number reads: {}".format(nreads))
print("read length: {}".format(readlength))
print("aligns 0: {}".format(float(aligns_0)/ nreads))
print("aligns 1: {}".format(float(aligns_1)/ nreads))
print("aligns 2: {}".format(float(aligns_2)/ nreads))

#--style_2
#--Overal comments are lacking. We asked you to explain what happens in important parts. E.g. what happens in a loop body,
#--such that we see that you understand what's going on.
#--END

#--writeup_2
#--Test data does never guarantee that it works on other data
#--Especially here when it is a relatively simple test data set
#--START
"""And thus the designed algorithm is guaranteed to work properly on the test data set."""
#--END