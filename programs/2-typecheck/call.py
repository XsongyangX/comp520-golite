#!/bin/python3
import constants
from constants import main
import os
"""
This file generates test files for function call type checking,
e.g.
	// valid
	var x,y,z int
	func foo(x,y,z int){}
	func main(){
		foo(x,y,z)
	}
	
	// invalid
	var x,y,z string
	func foo(x,y,z int){}
	func main(){
		foo(x,y,z)
	}
"""

FOLDER = "call/"
VALID_PATH = "valid/" + FOLDER
INVALID_PATH = "invalid/" + FOLDER
COMMENT_VALID = "/* The file contains correct function call typing. */"
COMMENT_INVALID = "/* The file contains incorrect function call typing. */"
FILE_NAME_BASE = "call_"

# output function
def output(statement, function, fileName, isValid = True):
	
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
		
		file.write(function + "\n")
		
		file.write(constants.MAIN)
		file.write("{")
		file.write("\n")
		
		file.write("\t" + statement)
		
		file.write("\n")
		file.write("}")
	return
	
# valid test files
def valid(destroy=False):
	
	for baseType in constants.BASE:
		function = "func foo(arg " + baseType + "){}"
		statement = "var x " + baseType + "\n\tfoo(x)"
		
		if not destroy:
			output(statement, function, FILE_NAME_BASE + baseType + ".go", isValid=True)
		else:
			os.remove(VALID_PATH + FILE_NAME_BASE + baseType + ".go")
	return
	
# invalid test files
def invalid(destroy=False):
	
	for funcArgType in constants.BASE:
	
		for callArgType in constants.BASE:
			
			# don't take valid assignments
			if funcArgType is callArgType:
				continue
			
			function = "func foo(arg " + funcArgType + "){}"
			statement = "var magic " + callArgType + "\n\tfoo(magic)"
				
			if not destroy:
				output(statement, function,\
					FILE_NAME_BASE + callArgType + "_func_" + funcArgType + ".go",\
					isValid = False)
			else:
				os.remove(INVALID_PATH + FILE_NAME_BASE + callArgType + "_func_" + funcArgType + ".go")
	return 

main(VALID_PATH, INVALID_PATH, valid, invalid)
