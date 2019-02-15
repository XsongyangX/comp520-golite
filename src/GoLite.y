/* This file contains the specifications of the grammar. */

/* Imports */
%{
	
	// TODO
	
	// Scanner line number
	extern int yylineno;
	
	// Scanner main function
	int yylex();
	
	// The yyerror function is called on any syntax error. When using %error-verbose, this function
	// takes a single argument (a string) which contains a detailed explanation of the error.
	// Otherwise it takes no arguments and it is up to the user to format the error using a minimal
	// description provided in yytext.
	// Thank you github repo.
	void yyerror(const char *s) {
		fprintf(stderr, "Error: (line %d) %s\n", yylineno, s);
		exit(1);
	}
%}

/* The union directive defines the yylval union used for associating (a) terminals (tokens)
 * returned by flex with additional scanning data (such as identifier/constant values); and
 * optionally (b) non-terminals (variables in productions) with AST information if any.
*/
%union {
	int intval;
	char *strval;
	float floatval;
	char *identifier;
}

// AST types
// TODO

/* Token directives define the token types to be returned by the scanner (excluding character
 * tokens). Each token definition takes [optionally, a reference to the associated field in the
 * yylval union] and an identifier. Multiple tokens can be defined per directive by using a list
 * of identifiers separated by spaces.
*/

// comments ignored

// automatic appending of semi-colon
%token tSEMICOLON

// Operators

// standard arithmetic
%token tPLUS tMINUS tTIMES tDIV tMODULO
// bit-wise arithmetic (?)
%token tAMPERSAND tVERTBAR tPOWER tLEFTSHIFT tRIGHTSHIFT tAMPPOWER
// enhanced assignment
%token tPLUSASSIGN tMINUSASSIGN tTIMESASSIGN tDIVASSIGN tMODASSIGN tAMPASSIGN tPOWASSIGN
%token tLSHIFTASSIGN tRSHIFTASSIGN tAMPPOWASSIGN
// logic
%token tAND tOR
// channeling
%token tLESSMINUS
// increment decrement
%token tINCREMENT tDECREMENT
// comparison
%token tEQUALS tLESS tGREATER tASSIGN tBANG tNOTEQUALS tLESSEQUALS tGREATEREQUALS
// colon assignment 
%token tCOLONASSIGN
// ellipse
%token tDOTS

// syntax
%token tLEFTPAREN tRIGHTPAREN tLEFTBRACE tRIGHTBRACE tLEFTBRACKET tRIGHTBRACKET
%token tCOLON tCOMMA tPERIOD

// keywords
%token tBREAK tCASE tCHAN tCONST tCONTINUE tDEFAULT tDEFER tELSE tFALLTHROUGH
%token tFOR tFUNC tGO tGOTO tIF tIMPORT tINTERFACE tMAP tPACKAGE tRANGE tRETURN
%token tSELECT tSTRUCT tSWITCH tTYPE tVAR

// Literals

// int
%token <intval> tINTDECVAL 
%token <strval> tINTOCTVAL tINTHEXVAL

// float
%token <floatval> tFLOATVAL

// rune
%token <identifier> tRUNEVAL

// string
%token <identifier> tSTRINGVAL tRAWSTRINGVAL

// unused in scanner (if any)

/* Precedence directives resolve grammar ambiguities by breaking ties between shift/reduce
 * operations. Tokens are grouped into precedence levels, with lower precedence coming first
 * and then higher precedence in later directives. Tokens specified in the same directive have
 * the same precedence. Ties at the same level are broken using either %left or %right, which
 * denote left-associative and right-associative respectively.
*/

/* Start */
%start programStart

/* For line numbers while parsing */
%locations

/* Error messages */
%error-verbose

/* AST header*/
// TODO (%code requires { #include "tree.h" }


/* Grammar rules */
%%

programStart: 
	;