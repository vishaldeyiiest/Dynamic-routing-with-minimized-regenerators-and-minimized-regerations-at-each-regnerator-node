from heuristics import read_paths, read_network, FSR

#option = int(raw_input('Enter 1 for FSR, 2 for RNRR, 3 for FSR-RNRR: '))

#network_file = '../../networks/' + (raw_input('Enter network file path (../../networks/): '))
test_file = '../../testing/arpanet20/1.txt'
network_file = '../../networks/arpanet.txt'
#Wavelengths = int(raw_input('Enter no of available Wavelengths in each link (enter 100 for non-blocking): '))
Wavelengths = 10
#R = int(raw_input('Enter optical reach: '))
R = 109
read_network(network_file, Wavelengths)
read_paths(network_file, R)

lines = []
for line in open(test_file): 
	a, b = map(int, line.rstrip('\n').split())
	lines.append((a, b))

print lines

for s,d in lines:
	moves = FSR(s, d)

	if moves == None:
		print 'No solution'
	else:
		print moves