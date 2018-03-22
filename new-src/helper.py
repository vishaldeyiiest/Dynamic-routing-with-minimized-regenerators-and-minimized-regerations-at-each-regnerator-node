import networkx as nx
from itertools import islice
import copy

fh = open("../networks/nsfnet.txt", 'rb')
G = nx.read_edgelist(fh, nodetype=int, data=(('weight', int), ))
fh.close()
# nx.write_edgelist(G, "test.edgelist")

def k_shortest_paths(G, src, dest, k):
	return islice(nx.shortest_simple_paths(G, src, dest), k)


''' compute k shortest paths for each pair 
'''
k = 5
nodes = len(G.node)
fp = open("nsfnet5.paths", 'w')

for src in range(1, nodes+1):
	for dest in range(1, nodes+1):
		if src != dest:
			for path in k_shortest_paths(G, src, dest, k):
				fp.write(' '.join(str(x) for x in path))
				fp.write('\n')

fp.close()
'''
S = 2
R = 2

D = [[[[0 for _ in range(R)] for _ in range(S)] for _ in range(R)] for _ in range(S)] 	# for disjoint primary
V = copy.deepcopy(D)		# for divergence check
L = copy.deepcopy(D)		# for disjointness of backup and primary
gain = copy.deepcopy(D)		# for gain computation

def search(a, x):
	for i in range(len(a)):
		if a[i] == x:
			return i
	return -1

def check_disjoint(a, b):
	set_a = set()
	set_b = set()
	for i in range(len(a)-1):
		set_a.add((a[i], a[i+1]))

	for i in range(len(b)-1):
		set_b.add((b[i], b[i+1]))

	return len(set_a.intersection(set_b))

def check_divergence(a, b):
	flag = 1
	for i in range(len(a)-1):
		k = search(b, a[i]) 
		if k >= 0 and k < len(b)-1:
			if b[k+1] != a[i+1]:
				flag = 0
				break
	return flag

# print check_divergence([1, 2, 3, 5], [2, 3, 6])

ppaths = [[[1, 3], [3, 5, 9]], [[4, 5, 9], [9, 11]]]
bpaths = [[[1, 5, 3], [3, 5, 8, 9]], [[4, 8, 9], [9, 10, 11]]]

for i in range(S):
	for j in range(R):
		for k in range(S):
			for l in range(R):
				if(i != k or (i == k and j != l)):
					gain[i][j][k][l] = check_disjoint(bpaths[i][j], bpaths[k][l])
					V[i][j][k][l] = check_divergence(bpaths[i][j], bpaths[k][l])

					if check_disjoint(ppaths[i][j], ppaths[k][l]) == 0:
						D[i][j][k][l] = 1
				if check_disjoint(ppaths[i][j], bpaths[k][l]) == 0:
					L[i][j][k][l] = 1

print L
'''