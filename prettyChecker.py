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
    with open(sys.argv[1], 'r') as file1, open(sys.argv[2], 'r') as file2:
        for i, (line1, line2) in enumerate(zip(file1,file2), 1):
            if line1 != line2:
                eprint("Files " + sys.argv[1] + " and " + sys.argv[2] + " differ on line " + str(i))
                sys.exit(1)

    return

if __name__ == "__main__":
    main()
