import copy

class State:

	def __init__(self, node, path, cost, reg_count = []):
		self.node = node
		self.paths = path
		self.cost = cost
		self.reg_count = reg_count
	'''
	def __str__(self):
		return str(self.paths)
	'''
	def __hash__(self):
		return hash(str(self.paths))

	def __eq__(self, another):
		return self.paths == another.paths