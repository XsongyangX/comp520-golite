#!/bin/python3
import constants
from constants import main
import os

"""
The script generates test files for type checking the assignment of
literals, like
	
	// Valid
	var x int
	var y int
	x = y
	
	// Invalid
	var x string
	var y float64
	x = y
"""

FOLDER = "assign/"
VALID_PATH = "valid/" + FOLDER
INVALID_PATH = "invalid/" + FOLDER
COMMENT_VALID = "/* The file contains valid assignment declared vars. */"
COMMENT_INVALID	= "/* The file contains invalid assignment of declared vars. */"
FILE_NAME_BASE = "assign_"

# output function
def output(statement, typedefs, fileName, isValid = True):
	
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
	
# valid test files
def valid(destroy=False):
	
	for baseType in constants.BASE:
		
		statement = "var magic " + baseType + "\n"\
			+ "\tvar m " + baseType + "\n"\
			+ "\tmagic = m"
		
		if not destroy:
			output(statement, "", \
				FILE_NAME_BASE + baseType + ".go", isValid=True)
		else:
			os.remove(VALID_PATH + FILE_NAME_BASE + baseType + ".go")
			
	for userType, typedef in constants.USER_TYPE.items():
		
		statement = "var magic " + userType + "\n"\
			+ "\tvar m " + userType + "\n"\
			+ "\tmagic = m"
		
		if not destroy:
			output(statement, typedef,  \
				FILE_NAME_BASE + baseType + ".go", isValid=True)
		else:
			os.remove(VALID_PATH + FILE_NAME_BASE + baseType + ".go")
	
	return
	
# invalid test files
def invalid(destroy=False):
	
	for baseType in constants.BASE:
	
		for baseType2 in constants.BASE:
			
			# don't take valid assignments
			if baseType2 == baseType:
				continue
				
			statement = "var magic " + baseType + "\n"\
				+ "\tvar m " + baseType2 + "\n"\
				+ "\tmagic = m"
				
			if not destroy:
				output(statement, "", \
					FILE_NAME_BASE + baseType + "_" + baseType2 + ".go",\
					isValid=False)
			else:
				os.remove(INVALID_PATH + FILE_NAME_BASE + baseType + "_" + baseType2 + ".go")
				
	for userType, typedef in constants.USER_TYPE.items():
	
		for userType2, typedef2 in constants.USER_TYPE.items():
		
			if userType == userType2:
				continue
				
			statement = "var magic " + userType + "\n"\
				+ "\tvar m " + userType2 + "\n"\
				+ "\tmagic = m"
				
			typdefs = typedef + "\n" + typedef2
			
			if not destroy:
				output(statement, typdefs,\
					FILE_NAME_BASE + userType + "_" + userType2 + ".go", isValid=False)
			
			else:
				os.remove(INVALID_PATH + FILE_NAME_BASE + userType + "_" + userType2 + ".go")
	
	return 
	
main(VALID_PATH, INVALID_PATH, valid, invalid)
