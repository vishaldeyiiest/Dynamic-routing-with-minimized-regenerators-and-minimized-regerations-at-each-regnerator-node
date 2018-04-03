from heuristics import read_paths, read_network, FSR, RNRR, FSR_RNRR
import time
#option = int(raw_input('Enter 1 for FSR, 2 for RNRR, 3 for FSR-RNRR: '))

#network_file = '../../networks/' + (raw_input('Enter network file path (../../networks/): '))
test_file = '../../testing/nsfnet500/1.txt'
network_file = '../../networks/nsfnet.txt'
#Wavelengths = int(raw_input('Enter no of available Wavelengths in each link (enter 100 for non-blocking): '))
Wavelengths = 120
#R = int(raw_input('Enter optical reach: '))
R = 350
read_network(network_file, Wavelengths)
read_paths(network_file, R)

lines = []
for line in open(test_file): 
	a, b = map(int, line.rstrip('\n').split())
	lines.append((a, b))

#print lines
block = 0


import numpy
def median(l):
    return numpy.median(numpy.array(l))

def std(l):
	return numpy.std(numpy.array(l))

start_time = time.time()

for s,d in lines:
	moves = FSR_RNRR(s, d)

	if moves == None:
		#print 'No solution for: ', s, d
		block += 1
	#else:
		#print moves

#print FSR(1, 11)	
from heuristics import reg_cost
print 'Regenerations: ', reg_cost
print "Total no of regenerations, MAX : ", sum(reg_cost.values()), max(reg_cost.values())
print "Blocked requests: ", block/500.0
print 'TIME required: ', time.time() - start_time

print 'Median: ', median(reg_cost.values())
print 'SD: ', std(reg_cost.values())