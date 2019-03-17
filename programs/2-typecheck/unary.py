#!/bin/python3
import constants
from constants import main
import os
import itertools

"""
This script generates test files for unary operations, like
	
	// valid
	var x bool
	var y bool
	x = !y
	
	// invalid
	var x bool
	var y string
	x = !y
"""

FOLDER = "unary/"
VALID_PATH = "valid/" + FOLDER
INVALID_PATH = "invalid/" + FOLDER
COMMENT_VALID = "/* The file contains valid unary operator usage. */"
COMMENT_INVALID	= "/* The file contains invalid unary operator usage. */"
FILE_NAME_BASE = "unary_"

def output(statement, typedefs, fileName, isValid=True):
	
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
		
		file.write(typedefs + "\n")
		
		file.write(constants.MAIN)
		file.write("{")
		file.write("\n")
		
		file.write("\t" + statement)
		
		file.write("\n")
		file.write("}")
	
	return

def valid(destroy=False):
	
	for op in constants.UNARY_OP:
		
		if op == '+' or op == '-':
			for numType, derivedTypes in constants.NUMERIC_TYPE.items():
				
				# get type definitions
				typedefs = "\n".join(constants.HIERARCHY[numType])
				
				derivedTypes.append(numType)
				
				# for each pair of types in the hierarchy
				for (magicType, xType) \
				in itertools.product(derivedTypes, repeat=2):
					statement = "var magic " + magicType + "\n"\
						+ "\tvar x " + xType + "\n"\
						+ "\tmagic = " + op + "x"
					

					if not destroy:
						output(statement, typedefs,\
							FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + magicType + "_" + xType + ".go",\
							isValid=True)
					else:
						os.remove(VALID_PATH + FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + magicType + "_" + xType + ".go")					
	
	return
	
def invalid(destroy=False):

	return
	
main(VALID_PATH, INVALID_PATH, valid, invalid)