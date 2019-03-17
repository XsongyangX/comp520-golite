#!/bin/python3
import constants
from constants import main
import os

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
				
				# for each type in the hierarchy
				for magicType in derivedTypes:
					statement = "var magic " + magicType + "\n"\
						+ "\tmagic = " + op + "magic"
					
					if not destroy:
						output(statement, typedefs,\
							FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + magicType + ".go",\
							isValid=True)
					else:
						os.remove(VALID_PATH + FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + magicType + ".go")					
		
		elif op == '!':
			for boolType, derivedTypes in constants.BOOL_TYPE.items():
				
				typedefs = "\n".join(constants.HIERARCHY[boolType])
				
				derivedTypes.append(boolType)
				
				# for each type in the hierarchy
				for magicType in derivedTypes:
					statement = "var magic " + magicType + "\n"\
						+ "\tmagic = " + op + "magic"
					
					if not destroy:
						output(statement, typedefs,\
							FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + magicType + ".go",\
							isValid=True)
					else:
						os.remove(VALID_PATH + FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + magicType + ".go")
			
		elif op == '^':
			for intType, derivedTypes in constants.INTEGER_TYPE.items():
			
				typedefs = "\n".join(constants.HIERARCHY[intType])
				
				derivedTypes.append(intType)
				
				# for each type in the hierarchy
				for magicType in derivedTypes:
					statement = "var magic " + magicType + "\n"\
						+ "\tmagic = " + op + "magic"
					
					if not destroy:
						output(statement, typedefs,\
							FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + magicType + ".go",\
							isValid=True)
					else:
						os.remove(VALID_PATH + FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + magicType + ".go")
	return
	
def invalid(destroy=False):

	return
	
main(VALID_PATH, INVALID_PATH, valid, invalid)