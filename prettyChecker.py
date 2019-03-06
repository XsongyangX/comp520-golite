#!/usr/bin/python3
from __future__ import print_function
"""
This script compares the content of two files.
It prints to stderr if the two files are not the same

"""

import sys


# taken from
# https://stackoverflow.com/questions/5574702/how-to-print-to-stderr-in-python
def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def main():

	# command line args
	# argv[1] = go file 1
	# argv[2] = go file 2
	if len(sys.argv) != 3:
		raise Exception("Run script like this: prettyChecker.py file1 file2")
	
	# open files
	file1 = open(sys.argv[1], "r").readlines()
	file2 = open(sys.argv[2], "r").readlines()
	
	for i in range(min(len(file1), len(file2))):
		if file1[i] != file2[i]:
			eprint("Files " + sys.argv[1] + " and " + sys.argv[2] + " differ on line " + str(i+1))
			sys.exit(1)

	return

if __name__ == "__main__":
	main()