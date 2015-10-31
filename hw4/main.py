import sys

import truss

if (len(sys.argv) < 3):

  print('Usage:')
  print('  python {} [joints file] [beams file] [optional plot output file]'.format(sys.argv[0]))
  exit()

joint_file = sys.argv[1]
beam_file = sys.argv[2]
output_file = None

if len(sys.argv) >= 4:
	output_file = sys.argv[3]

try:
  t = truss.Truss(joint_file, beam_file, output_file)
except Exception as e:
  print('ERROR: {}'.format(e))
  exit()

print(t)