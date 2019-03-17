"""
Constants used to generate test files
"""
import os, sys

PACKAGE = "package main"
BASE = ["int", "float64", "bool", "rune", "string"]
LITERALS = {"int":300, "float64":10.32, "bool":"false", "rune":"'3'", "string": "\"hello world\""}
MAIN = "func main()"

USER_TYPE = {"num":"type num int",\
	"exception": "type exception string",
	"long": "type long float64",\
	"Date": "type Date struct{ day string; month string; year int; }"}

# typedefs for nested numeric type
HIERARCHY = {"int": ["type num int", "type count num", "type quantity count"],\
	"float64": ["type real float64", "type positive real", "type non_zero positive"],\
	"rune": ["type letter rune", "type character letter", "type symbol character"]}

# dict = type : array of derived types
NUMERIC_TYPE = {"int": ["num", "count", "quantity"],\
	"float64" : ["real", "positive", "non_zero"],\
	"rune" : ["letter", "character", "symbol"]}
INTEGER_TYPE = {"int": ["num", "count", "quantity"],\
	"rune" : ["letter", "character", "symbol"]}

UNARY_OP = ["+", "-", "!", "^"]
UNARY_OP_TO_WORD = {"+":"plus", "-":"minus", "!":"not", "^":"bitneg"}
BINARY_OP = ["||", "&&", "==", "!=", "<", "<=", ">", ">=",\
	"+", "-", "*", "/", "%", "|", "&", "<<", ">>", "&^", "^"]

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