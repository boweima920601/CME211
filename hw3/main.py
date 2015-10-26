import sys

import airfoil

if len(sys.argv) < 2:
  print('Usage:')
  print('  python {} <airfoil data directory>'.format(sys.argv[0]))
  exit()

inputdir = sys.argv[1]

try:
  a = airfoil.Airfoil(inputdir)
except Exception as e:
  print('ERROR: {}'.format(e))
  exit()

print(a)
