#!/bin/python3
import constants
from constants import main
import os

"""
The script generates test files for type checking the assignment of
literals, like
	
	// Valid
	var x int
	x = 3
	
	// Invalid
	var y string
	y = 0.0
"""

FOLDER = "assign_literal/"
VALID_PATH = "valid/" + FOLDER
INVALID_PATH = "invalid/" + FOLDER
COMMENT_VALID = "/* The file contains valid assignment of literals. */"
COMMENT_INVALID	= "/* The file contains invalid assignment of literals. */"
FILE_NAME_BASE = "assign_"

# output function
def output(assignmentStatement, fileName, isValid = True):
	
	# choose valid or invalid folder
	correctFolder = ""
	
	if isValid:
		correctFolder = "valid"
	else:
		correctFolder = "invalid"
		
	with open(correctFolder + "/" + FOLDER + "/" + fileName, "w") as file:
		
		# beginning comment
		if isValid:
			file.write(COMMENT_VALID)
		else:
			file.write(COMMENT_INVALID)
		file.write("\n")
		file.write(constants.PACKAGE)
		file.write("\n\n")
		
		file.write(constants.MAIN)
		file.write("{")
		file.write("\n")
		
		file.write("\t" + assignmentStatement)
		
		file.write("\n")
		file.write("}")
	
	return