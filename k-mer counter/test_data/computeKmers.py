import sys

fin = open(sys.argv[1], 'r')
k = int(sys.argv[2])

seq = fin.read()

if seq[-1] == '\n':
    seq = seq[:-1]

kmers = []
for i in range(len(seq)-(k-1)):
  if not (i%((len(seq)-(k-1))/10)):
    print "."
  curKmer = seq[i:i+k]
  kmers.append(curKmer)
fin.close()

kmers_sorted = sorted(kmers)
curKmer = kmers_sorted[0]
curCount = 1
fout = open(sys.argv[3], 'w')
for i in range(1, len(kmers_sorted)):
  if kmers_sorted[i] == curKmer:
    curCount += 1
  else:
    fout.write(curKmer+" "+str(curCount)+"\n")
    curKmer = kmers_sorted[i]
    curCount = 1
fout.write(curKmer+" "+str(curCount)+"\n")

fout.close()
