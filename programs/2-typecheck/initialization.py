#!/bin/python3
import constants
import os, sys
"""
This file generates test files for initialization operations

"""

FOLDER = "initialization/"
VALID_PATH = "valid/" + FOLDER
INVALID_PATH = "invalid/" + FOLDER
COMMENT_VALID = "/* The file contains correct var initialization types. */"
COMMENT_INVALID = "/* The file contains incorrect var initialization types. */"
FILE_NAME_BASE = "var_assign_"

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
		file.write("\n")
		
		file.write(constants.MAIN)
		file.write("\n")
		file.write("{")
		
		file.write("\t" + assignmentStatement)
		
		file.write("\n")
		file.write("}")

# valid test files
def valid(destroy=False):
	
	assignmentStatement = ""
	
	for baseType in constants.BASE:
		
		assignmentStatement = "var magic " \
			+ baseType + " = " + str(constants.LITERALS[baseType])
		
		if not destroy:
			output(assignmentStatement, \
				FILE_NAME_BASE + baseType + ".go", isValid=True)
		else:
			os.remove(VALID_PATH + FILE_NAME_BASE + baseType + ".go")
	return
	
# invalid test files
def invalid(destroy=False):
	
	assignmentStatement = ""
	
	for baseType in constants.BASE:
	
		for literalType in constants.LITERALS.keys():
			
			# don't take valid assignments
			if literalType == baseType:
				continue
				
			assignmentStatement = "var magic " \
				+ baseType + " = " + str(constants.LITERALS[literalType])
				
			if not destroy:
				output(assignmentStatement, \
					FILE_NAME_BASE + baseType + "_" + literalType + ".go",
					isValid=False)
			else:
				os.remove(INVALID_PATH + FILE_NAME_BASE + baseType + "_" + literalType + ".go")
	return 
	
def main():
	
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

if __name__ is "__main__":
	main()
main()