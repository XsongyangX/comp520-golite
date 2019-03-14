"""
Constants used to generate test files
"""
import os, sys

PACKAGE = "package main"
BASE = ["int", "float64", "bool", "rune", "string"]
LITERALS = {"int":300, "float64":10.32, "bool":"false", "rune":"'3'", "string": "\"hello world\""}
MAIN = "func main()"
		
def main(VALID_PATH, INVALID_PATH, valid, invalid):
	
	# check for directories
	if not os.path.isdir(VALID_PATH):
		os.mkdir(VALID_PATH)
	
	if not os.path.isdir(INVALID_PATH):
		os.mkdir(INVALID_PATH)
	
	# first command-line argument is absent
	if len(sys.argv) == 1:
		# generate
		valid()
		invalid()
		
	# first command-line argument is "destroy"
	elif sys.argv[1] == "destroy":
		# destroy
		valid(destroy=True)
		invalid(destroy=True)
		
	return