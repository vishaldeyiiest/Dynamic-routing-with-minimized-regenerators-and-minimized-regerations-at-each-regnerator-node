import random

with open("1.txt", mode="r") as myFile:
    lines = myFile.read().splitlines()

random.shuffle(lines)

for i in lines:
	print i







