import copy
from state_model import State
from sys import maxint
from math import ceil
import networkx as nx
from itertools import islice

W, R = 0, 0
N = 0
#network_file = "../../networks/arpanet.txt"
#path_file = "arpanet5.paths"
reg = [3, 4, 11, 9, 13, 20, 6, 16, 8]
path_list = []

W_link = {} # dictionary to hold wavelengths that can be assigned to link
dist = []

def read_network(network_file, Wav):
	fh = open(network_file, 'rb')
	G = nx.read_edgelist(fh, nodetype=int, data=(('weight', int), ), create_using = nx.DiGraph())
	fh.close()
	edges = list(G.edges)
	edges.extend(G.reverse().edges)

	global N 
	N = len(G.nodes)

	global W
	W = Wav
	
	t = [maxint for _ in range(N+1)]
	for _ in range(N+1):
		dist.append(copy.deepcopy(t))
	#print edges
	for e in edges:
		W_link[e] = set(copy.deepcopy(range(1, W+1)))
	#print W_link
	length = nx.all_pairs_dijkstra_path_length(G)

	for i, l in length:
		for dest in l:
			dist[i][dest] = l[dest]


def read_paths(network_file, Reach):
	global R
	R = Reach
	fh = open(network_file, 'rb')
	G = nx.read_edgelist(fh, nodetype=int, data=(('weight', int), ))
	paths = nx.all_pairs_dijkstra_path(G, R)
	fh.close()

	for i, p in paths: 
		for dest in p:
			if i != dest:
				path_list.append(p[dest])


def path_to_reg(node):
	path_reg = []
	#print path_list	
	for p in path_list:
		if p[0] == node and p[-1] in reg:
			path_reg.append(p)
	return path_reg

def path_to_dest(s, d):
	path_d = []
	for p in path_list:
		#print p
		if p[0] == s and p[-1] == d:
			path_d.append(p)
	return path_d


def assign_list_colors(paths):
	Wpaths = [] 

	for p in paths:
		wp = W_link[(p[0], p[1])]
		for i in range(1, len(p)-1):
			wp = wp & W_link[(p[i], p[i+1])]
		
		Wpaths.append(wp)
	return Wpaths

def find_edge(patha, pathb):
	set_a = set()
	set_b = set()
	for i in range(len(patha)-1):
		set_a.add((patha[i], patha[i+1]))
	for i in range(len(pathb)-1):
		set_b.add((pathb[i], pathb[i+1]))

	return len(set_a & set_b)

def update_wav_list(paths, Wassign):
	for j in range(len(paths)):
		p = paths[j]
		#print p
		for i in range(len(p)-1):
			if Wassign[j] in W_link[(p[i], p[i+1])]:
				W_link[(p[i], p[i+1])].remove(Wassign[j])

	#Wpaths = assign_list_colors(Wpaths)


def assign_wavelength(paths, Wpaths):
	Wassign = [0 for _ in paths]
	Wassign[0] = Wpaths[0].pop()

	for i in range(1, len(paths)):
		if find_edge(paths[i], paths[i-1]):
			Wpaths[i].remove(Wassign[i-1])
		if Wpaths[i]:
			Wassign[i] = Wpaths[i].pop()
		else:
			return False

	update_wav_list(paths, Wassign)
	print 'Wavelengths assigned: ',  Wassign
	return True


def compute_cost_FSR(node, paths, dest):
	hcost = int(ceil(dist[node][dest]/R))
	#print 'Dist: ', node, dest, dist[node][dest]
	gcost = len(paths) - 1
	return hcost + gcost

#def compute_cost_RNRR`(state):

#def compute_cost_FSRRNRR(state):

def compare_cost(state1, state2):
	return state1.cost - state2.cost 

def create_new_states(state, dest, visited, node_visited):
	path_new = path_to_reg(state.node)

	if dist[state.node][dest] <= R:
		path_dest = path_to_dest(state.node, dest)	
		path_dest.extend(path_new)
		path_new = path_dest
	#print "Paths: ", path_new
	new_states = []
	#print path_new
	for e in path_new:
		if e[-1] not in node_visited:
			temp_paths = copy.deepcopy(state.paths)
			temp_paths.append(e)
			#print temp_paths
			hc = compute_cost_FSR(e[-1], temp_paths, dest)
			#print e[-1], hc
			temp1 = State(e[-1], temp_paths, hc)	
			if temp1 not in visited:
				new_states.append(temp1)

	return new_states


def FSR(src, dest):

	visited = set()
	node_visited = set()
	start = State(src, [], compute_cost_FSR(src, [], dest))
	states = [start]

	while True:
		if len(states) == 0:
			return None
		
		current_state = states.pop(0)
		#print current_state
		visited.add(current_state)
		node_visited.add(current_state.node)

		if current_state.node == dest:
			#print current_state.paths
			Wp = assign_list_colors(current_state.paths)
			assign_wavelength(current_state.paths, Wp)

			return current_state.paths
	
		states.extend(create_new_states(current_state, dest, visited, node_visited))
		states.sort(compare_cost)


#def RNRR(src, dest):
