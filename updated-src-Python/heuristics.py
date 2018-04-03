import copy
from state_model import State
from sys import maxint
from math import ceil
import networkx as nx
from itertools import islice

W, R = 0, 0
N = 0

Seg = 4		# Max number of alowed Segments
RN = 125		# Max number of regenerations at a node
'''
FOR ARPANET
network_file = "../../networks/arpanet.txt"
'''
#reg = [3, 4, 11, 9, 13, 20, 6, 16, 8]

#reg = [8, 6, 15, 16]
'''
for NSFNET
network_file = "../../networks/arpanet.txt"
'''
reg = [6, 7, 9]
reg_cost = {}
for r in reg:
	reg_cost[r] = 0

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
	fh = open(network_file, 'rb')
	G = nx.read_edgelist(fh, nodetype=int, data=(('weight', int), ))
	fh.close()
	length = nx.all_pairs_dijkstra_path_length(G)

	for i, l in length:
		for dest in l:
			#print i, dest, l[dest]
			dist[i][i] = 0
			dist[i][dest] = l[dest]
			dist[dest][i] = l[dest]
	#print dist

def read_paths(network_file, Reach):
	global R
	R = Reach
	fh = open(network_file, 'rb')
	G = nx.read_edgelist(fh, nodetype=int, data=(('weight', int), ))
	paths = nx.all_pairs_dijkstra_path(G, R)
	fh.close()

	#fout = open('nsfnet.paths', 'w')
	for i, p in paths: 
		for dest in p:
			if i != dest:
				path_list.append(p[dest])
				#fout.write(' '.join(str(x) for x in p[dest]))
				#fout.write('\n')
	#fout.close()

def path_to_reg(node):
	path_reg = []
	#print path_list	
	for p in path_list:
		if p[0] == node and p[-1] in reg and set(p[1:-1]) - set(reg) == set(p[1:-1]) :
			path_reg.append(p)
	return path_reg

def path_to_dest(s, d):
	path_d = []
	for p in path_list:
		#print p
		if p[0] == s and p[-1] == d and set(p[1:-1]) - set(reg) == set(p[1:-1]):
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

def update_reg_cost(regs):
	for r in regs:
		reg_cost[r] += 1

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
	if Wpaths[0]:
		Wassign[0] = Wpaths[0].pop()
	else:
		return None

	for i in range(1, len(paths)):
		if find_edge(paths[i], paths[i-1]):
			Wpaths[i].remove(Wassign[i-1])
		if Wpaths[i]:
			Wassign[i] = Wpaths[i].pop()
		else:
			return None

	return Wassign


def compute_cost_FSR(node, paths, dest):
	hcost = int(ceil(dist[node][dest]/R))
	#print 'Dist: ', node, dest, dist[node][dest]
	gcost = len(paths) - 1
	return hcost + gcost

#def compute_cost_RNRR`(state):

#def compute_cost_FSRRNRR(state):

def compare_cost_FSR(state1, state2):
	return state1.cost - state2.cost 

def compare_cost_RNRR(state1, state2):
	reg_count1 = []
	reg_count2 = []
	for r in state1.regs:
		reg_count1.append(reg_cost[r]) 
	for r in state2.regs:
		reg_count2.append(reg_cost[r]) 
	if reg_count1 < reg_count2:
		return -1
	elif reg_count1 > reg_count2:
		return 1
	else:
		return 0

def compare_cost_FSR_RNRR(state1, state2):
	#if compare_cost_FSR(state1, state2) == 0:
	#	return compare_cost_RNRR(state1, state2)
	#else:
	return compare_cost_FSR(state1, state2) + compare_cost_RNRR(state1, state2)

def max_cost(regs):
	if not regs:
		return 0
	cost_v = {}
	for i in regs:
		cost_v[i] = reg_cost[i] + 1
	return max(cost_v.values())

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
			temp_regs = copy.deepcopy(state.regs)
			if e[-1] != dest:
				temp_regs.append(e[-1])

			Wp = assign_list_colors(temp_paths)
			Wassign = assign_wavelength(temp_paths, Wp)
			if Wassign is not None and Seg >= len(temp_paths)-1 and max_cost(temp_regs) <= RN:	
				temp_state = State(e[-1], temp_paths, hc, Wassign, temp_regs)

				if temp_state not in visited:
					new_states.append(temp_state)

	return new_states


def FSR(src, dest):

	visited = set()
	node_visited = set()
	start = State(src, [], compute_cost_FSR(src, [], dest), [])
	states = [start]

	while True:
		if len(states) == 0:
			return None
		#print len(states)
		current_state = states.pop(0)
		#print current_state.paths
		visited.add(current_state)
		node_visited.add(current_state.node)

		if current_state.node == dest:
			#print current_state.paths
				
			update_reg_cost(current_state.regs)
			#print 'Wavelengths Assigned: ', current_state.wavelengths
			return current_state.paths
	
		states.extend(create_new_states(current_state, dest, visited, node_visited))
		states.sort(compare_cost_FSR)


def RNRR(src, dest):

	visited = set()
	node_visited = set()
	start = State(src, [], compute_cost_FSR(src, [], dest), [])
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
			#Wp = assign_list_colors(current_state.paths)
			#assign_wavelength(current_state.paths, Wp)
			update_reg_cost(current_state.regs)

			return current_state.paths, reg_cost
	
		states.extend(create_new_states(current_state, dest, visited, node_visited))
		states.sort(compare_cost_RNRR)


def FSR_RNRR(src, dest):

	visited = set()
	node_visited = set()
	start = State(src, [], compute_cost_FSR(src, [], dest), [])
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
			#Wp = assign_list_colors(current_state.paths)
			#assign_wavelength(current_state.paths, Wp)
			update_reg_cost(current_state.regs)

			return current_state.paths
	
		states.extend(create_new_states(current_state, dest, visited, node_visited))
		states.sort(compare_cost_FSR_RNRR)