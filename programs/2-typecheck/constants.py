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

# typedefs for nested type
HIERARCHY = {"int": ["type num int", "type count num", "type quantity count"],\
	"float64": ["type real float64", "type positive real", "type non_zero positive"],\
	"rune": ["type letter rune", "type character letter", "type symbol character"],\
	"bool": ["type answer bool", "type safe answer", "type exist answer"],\
	"string": ["type error string", "type log error", "type report log"],\
	"struct": ["type compound struct { x int; y float64;}",\
		"type composed struct { c compound; s string;}",\
		"type group struct { c1 compound; c2 composed;}"],\
	"array": ["type inbox [100]string", "type mailbox [3]inbox", "type postoffice [5]mailbox"],\
	"slice": ["type ints []int", "type strings []strings", "type runecrafter []rune"]}
	
# dict = type : array of derived types
NUMERIC_TYPE = {"int": ["num", "count", "quantity"],\
	"float64" : ["real", "positive", "non_zero"],\
	"rune" : ["letter", "character", "symbol"]}
INTEGER_TYPE = {"int": ["num", "count", "quantity"],\
	"rune" : ["letter", "character", "symbol"]}
BOOL_TYPE = {"bool": ["answer", "safe", "exist"]}
STRING_TYPE = {"string": ["error", "log", "report"]}
STRUCT_TYPE = {"struct": ["compound", "composed", "group"]}
ARRAY_TYPE = {"array": ["inbox", "mailbox", "postoffice"]}
SLICE_TYPE = {"slice": ["ints", "strings", "runecrafter"]}

UNARY_OP = ["+", "-", "!", "^"]
UNARY_OP_TO_WORD = {"+":"plus", "-":"minus", "!":"not", "^":"bitneg"}
BINARY_OP = ["||", "&&", "==", "!=", "<", "<=", ">", ">=",\
	"+", "-", "*", "/", "%", "|", "&", "<<", ">>", "&^", "^"]
BINARY_OP_TO_WORD = {"||":"or", "&&":"and", "==":"eq", "!=":"neq",\
	"<":"less", "<=":"leq", ">":"greater", ">=": "geq",\
	"+":"plus", "-":"minus", "*":"times", "/": "div", "%":"mod",\
	"|": "bitor", "&": "bitand", "<<":"lshift", ">>":"rshift",\
	"&^":"bitandnot", "^":"bitxor"}

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