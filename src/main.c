/* 
Main file of the GoLite compiler
*/

#include <stdio.h>
#include <string.h>

void yyparse();
int yylex();
int isToken;

int main(int argc, char** argv)
{
    // check if an argument is provided
	if (!(argc == 2 || argc == 3))
	{
		fprintf(stderr, 
			"Must provide an argument: scan|tokens|parse|pretty|symbol|typecheck|codegen\n");
		fprintf(stderr,
			"In the case of codegen, a file name is required as a 2nd argument\n");
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
	/*
	else if ( 0 == strcmp(argv[1], "parse"))
	{
		isToken = 0;
		yyparse();
		printf("OK\n");
		return 0;
	}
	else if ( 0 == strcmp(argv[1], "pretty"))
	{
		isToken = 0;
		yyparse();
		prettyProgram(root, 0); // 0 for no tabs so far
		return 0;
	}
	else if ( 0 == strcmp(argv[1], "symbol"))
	{
		isToken = 0;
		g_symbols = 1;
		yyparse();
		symbolFromProgramStart(root);
		return 0;
	}
	else if ( 0 == strcmp(argv[1], "typecheck"))
	{
		isToken = 0;
		g_symbols = 0;
		yyparse();
		symbolFromProgramStart(root);
		printf("OK\n");
		return 0;
	}
	else if ( 0 == strcmp(argv[1], "codegen"))
	{
		isToken = 0;
		g_symbols = 0;
		yyparse();
		symbolFromProgramStart(root);
		emitToFile(argv[2]);
		emitProgramStart(root);
		printf("OK\n");
		return 0;
	}
	*/
	else 
	{
		fprintf(stderr, 
			"Valid arguments are scan|tokens|parse|pretty|symbol|typecheck|codegen\n");
		fprintf(stderr,
			"In the case of codegen, a file name is required as a 2nd argument\n");
		return 1;
	}

} 