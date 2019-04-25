#!/bin/python3
import constants
from constants import main
import os

"""
This script generates test files for assigning function return
values.
"""
FOLDER = "return/"
VALID_PATH = "valid/" + FOLDER
INVALID_PATH = "invalid/" + FOLDER
COMMENT_VALID = "/* The file contains correct assignments of return values. */"
COMMENT_INVALID = "/* The file contains incorrect assignments of return values. */"
FILE_NAME_BASE_VAR = "var_assign_"
FILE_NAME_BASE_SIMPLE = "assign_"
FILE_NAME_BASES = [FILE_NAME_BASE_VAR, FILE_NAME_BASE_SIMPLE]

# output function 
def output(statement, function, fileName, isValid=True):
	
	# choose folder
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
	
		# package main
		file.write("\n" + constants.PACKAGE + "\n")
		
		# defined function
		file.write(function)
		
		# main
		file.write("\n" + constants.MAIN + "{\n")
		file.write("\t" + statement + "\n")
		file.write("}")
		
	return

# valid test files
def valid(destroy=False):
	
	# form a function for each return type
	for functionReturnType in constants.BASE:
		function = "func foo() " + functionReturnType + " {\n"\
			+ "\treturn " + str(constants.LITERALS[functionReturnType])\
			+ "\n}"
		
		# for the corresponding statement
		for fileNameBase in FILE_NAME_BASES:
			statement = ""

			if fileNameBase is FILE_NAME_BASE_SIMPLE:
				statement = "var a " + functionReturnType + "; " + "a = foo()"
			elif fileNameBase is FILE_NAME_BASE_VAR:
				statement = "var a " + functionReturnType + " = foo()"
			else:
				raise Exception
				
			if not destroy:
				output(statement, function,\
					fileNameBase + functionReturnType + "_func" + ".go",\
					isValid=True)
			else:
				os.remove(VALID_PATH + fileNameBase + functionReturnType + "_func" + ".go")	
	return

# invalid test files
def invalid(destroy=False):
	
	# form a function for each return type
	for functionReturnType in constants.BASE:
		function = "func foo() " + functionReturnType + " {\n"\
			+ "\treturn " + str(constants.LITERALS[functionReturnType])\
			+ "\n}"
			
		# form all non-corresponding statement
		for varType in constants.BASE:
			
			if varType is functionReturnType:
				continue
				
			# for the corresponding statement
			for fileNameBase in FILE_NAME_BASES:
				statement = ""

				if fileNameBase is FILE_NAME_BASE_SIMPLE:
					statement = "var a " + varType + "; " + "a = foo()"
				elif fileNameBase is FILE_NAME_BASE_VAR:
					statement = "var a " + varType + " = foo()"
				else:
					raise Exception
					
				if not destroy:
					output(statement, function,\
						fileNameBase + varType + "_func_" + functionReturnType + ".go",\
						isValid=False)
				else:
					os.remove(VALID_PATH + fileNameBase + varType + "_func_" + functionReturnType + ".go")
			
	
	return

main(VALID_PATH, INVALID_PATH, valid, invalid)