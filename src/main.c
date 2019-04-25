/* 
Main file of the GoLite compiler
*/

#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "pretty.h"
#include "weeder.h"
#include "symbol.h"
#include "typecheck.h"
#include "codegen.h"

void yyparse();
int yylex();
int isToken;
int symbolPrint;
PROGRAM *my_prog;

int main(int argc, char** argv)
{
    // check if an argument is provided
	if (!(argc == 2) && !(argc == 3))
	{
		fprintf(stderr, 
			"Must provide an argument: scan|tokens|parse|pretty|symbol|typecheck|codegen\n");
		return 1;
	}

	// get command line argument
	if ( 0 == strcmp(argv[1], "scan"))
	{
		isToken = 0;
		yylex();
		printf("OK\n");
		return 0;
	}
	else if ( 0 == strcmp(argv[1], "tokens"))
	{
		isToken = 1;
		while (yylex()) {}
		return 0;
	}
#ifndef SCAN_TEST
	else if ( 0 == strcmp(argv[1], "parse"))
	{
		isToken = 0;
		yyparse();
		weedRoot(my_prog);
		printf("OK\n");
		return 0;
	}
	
	else if ( 0 == strcmp(argv[1], "pretty"))
	{
		isToken = 0;
		yyparse();
		weedRoot(my_prog);
		prettyPROG(my_prog); 
		return 0;
	}
#endif
#if !defined(SCAN_TEST) && !defined(PRETTY_TEST)
	else if ( 0 == strcmp(argv[1], "symbol"))
	{
		isToken = 0;
		symbolPrint = 1;
		yyparse();
		weedRoot(my_prog);
		checkProg(my_prog);
		return 0;
	}
	else if ( 0 == strcmp(argv[1], "typecheck"))
	{
		isToken = 0;
		symbolPrint = 0;
		yyparse();
		weedRoot(my_prog);
		checkProg(my_prog);
		typeProgram(my_prog);
		printf("OK\n");
		return 0;
	}
	else if ( 0 == strcmp(argv[1], "codegen"))
	{
		isToken = 0;
		symbolPrint = 0;
		yyparse();
		weedRoot(my_prog);
		checkProg(my_prog);
		typeProgram(my_prog);
		codegenProgram(my_prog, NULL);
		fprintf(stderr, "OK\n");
		return 0;
	}
	
#endif
	else 
	{
		fprintf(stderr, 
			"Valid arguments are scan|tokens|parse|pretty|symbol|typecheck|codegen\n");

		return 1;
	}

} 
