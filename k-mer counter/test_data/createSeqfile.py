import sys
import random

fout = open(sys.argv[1], 'w')

for i in range(int(sys.argv[2])):
    fout.write(random.choice("ACGTN"))

fout.close()
