import copy

class State:

	def __init__(self, node, path, cost, wavelengths = [], regs = []):
		self.node = node
		self.paths = path
		self.cost = cost
		self.regs = regs
		self.wavelengths = wavelengths
	'''
	def __str__(self):
		return str(self.paths)
	'''
	def __hash__(self):
		return hash(str(self.paths))

	def __eq__(self, another):
		return self.paths == another.paths