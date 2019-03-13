#!/bin/python3

"""
This file generates test files for initialization operations

"""

# constants
PACKAGE = "package main"
BASE = ["int", "float64", "bool", "rune", "string"]
LITERALS = {"int":300, "float64":10.32, "bool":"false", "rune":"'3'", "string": "hello world"}

# Generate invalid initialization
def invalid():
	assignmentStatement = "var magic"
	
	for baseType in BASE:
		assignmentStatement = assignmentStatement + " " + baseType
	
	return
	
def main():

	return

if __name__ is "__main__":
	main()