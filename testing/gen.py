import random

req = []
N = int(raw_input('Enter N: '))

for i in range(N):
	req.append(random.sample(range(1, 22), 2))

fh = open('1.txt', 'w')
for r in req:
	fh.write(' '.join(str(x) for x in r))
	fh.write('\n')
