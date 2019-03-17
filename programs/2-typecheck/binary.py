#!/bin/python3
import constants
from constants import main
import os

"""
This script generates test files for binary operations, like
	
	// valid
	var x bool
	var y bool
	var magic bool
	magic = x && y
	
	// invalid
	var x bool
	var y string
	var magic int
	magic = x + y
"""

FOLDER = "binary/"
VALID_PATH = "valid/" + FOLDER
INVALID_PATH = "invalid/" + FOLDER
COMMENT_VALID = "/* The file contains valid binary operator usage. */"
COMMENT_INVALID	= "/* The file contains invalid binary operator usage. */"
FILE_NAME_BASE = "binary_"

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
	
	for op in constants.BINARY_OP:
		
		# logical
		if op in ['||', '&&']:
			
			typedefs = "\n".join(constants.HIERARCHY["bool"])
			
			for boolType in constants.BOOL_TYPE["bool"] + ["bool"]:
			
				statement = "var x " + boolType + "\n"\
					+ "\tvar y " + boolType + "\n"\
					+ "\tvar magic " + boolType + "\n"\
					+ "\tmagic = x " + op + " y\n"
				
				if not destroy:
					output(statement, typedefs,\
						FILE_NAME_BASE + constants.BINARY_OP_TO_WORD[op] + "_" + boolType + ".go",\
						isValid=True)
				else:
					os.remove(VALID_PATH + FILE_NAME_BASE + constants.BINARY_OP_TO_WORD[op] + "_" + boolType + ".go")
		
		# comparable
		elif op in ['==', '!=']:
		
			typedefs = "\n".join(\
				constants.HIERARCHY["bool"]\
				+ constants.HIERARCHY["float64"]\
				+ constants.HIERARCHY["rune"]\
				+ constants.HIERARCHY["int"]\
				+ constants.HIERARCHY["string"])
				
				
	return

def invalidFilesForOp(op, typeRange, destroy=False):

	for baseType in typeRange:
		statement = "var magic " + baseType + "\n"\
			+ "\tmagic = " + op + "magic"
				
		if not destroy:
			output(statement, "",\
				FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + baseType + ".go",\
				isValid=False)
		else:
			os.remove(INVALID_PATH + FILE_NAME_BASE + constants.UNARY_OP_TO_WORD[op] + "_" + baseType)

	return 
	
def invalid(destroy=False):

	for op in constants.UNARY_OP:
	
		if op == '+' or op == '-':
			invalidFilesForOp(op, ['bool', 'string'], destroy=False)
			
		elif op == '!':
			invalidFilesForOp(op, ['int', 'float64', 'rune', 'string'], destroy=False)
		
		elif op == '^':
			invalidFilesForOp(op, ['float64', 'bool', 'string'], destroy=False)
		
		else:
			raise Exception
	return
	
main(VALID_PATH, INVALID_PATH, valid, invalid)