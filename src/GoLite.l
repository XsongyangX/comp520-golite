
%{
#include "y.tab.h"
#include <string.h>
#include <stdlib.h>

// Stores the line number with each token sent to bison
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
extern int isToken;
int requiresIns = 0;
%}

/* Scanner options, line number generation */
%option yylineno

/* Removes warnings about input/output functions */
%option noinput
%option nounput

/* Helper */
PUNCTUATION [?<>+-=,^$|&]

%%
[ \t\r]+ 
"\n" {if(requiresIns){ requiresIns = 0; return tSEMICOLON;}}

"+" { // OPERATORS
	if(isToken){printf("tPLUS\n");} return '+';}
"-" {if(isToken){printf("tMINUS\n");} return '-';}
"*" {if(isToken){printf("tTIMES\n");} return '*';}
"/" {if(isToken){printf("tDIV\n");} return '/';}
"%" {if(isToken){printf("tMODULO\n");} return '%';}
"&" {if(isToken){printf("tAMPERSAND\n");} return '&';}
"|" {if(isToken){printf("tVERTBAR\n");} return '|';}
"^" {if(isToken){printf("tPOWER\n");} return '^';}
"<<" {if(isToken){printf("tLEFTSHIFT\n");} return tLEFTSHIFT;}
">>" {if(isToken){printf("tRIGHTSHIFT\n");} return tRIGHTSHIFT;}
"&^" {if(isToken){printf("tAMPPOWER\n");} return tAMPPOWER;}
"+=" {if(isToken){printf("tPLUSASSIGN\n");} return tPLUSASSIGN;}
"-=" {if(isToken){printf("tMINUSASSIGN\n");} return tMINUSASSIGN;}
"*=" {if(isToken){printf("tTIMESASSIGN\n");} return tTIMESASSIGN;}
"/=" {if(isToken){printf("tDIVASSIGN\n");} return tDIVASSIGN;}
"%=" {if(isToken){printf("tMODASSIGN\n");} return tMODASSIGN;}
"&=" {if(isToken){printf("tAMPASSIGN\n");} return tAMPASSIGN;}
"^=" {if(isToken){printf("tPOWASSIGN\n");} return tPOWASSIGN;}
"<<=" {if(isToken){printf("tLSHIFTASSIGN\n");} return tLSHIFTASSIGN;}
">>=" {if(isToken){printf("tRSHIFTASSIGN\n");} return tRSHIFTASSIGN;}
"&^=" {if(isToken){printf("tAMPPOWASSIGN\n");} return tAMPPOWASSIGN;}
"&&" {if(isToken){printf("tAND\n");} return tAND;}
"||" {if(isToken){printf("tOR\n");} return tOR;}
"<-" {if(isToken){printf("tLESSMINUS\n");} return tLESSMINUS;}
"++" {if(isToken){printf("tINCREMENT\n");} requiresIns = 1; return tINCREMENT;}
"--" {if(isToken){printf("tDECREMENT\n");} requiresIns = 1; return tDECREMENT;}
"==" {if(isToken){printf("tEQUALS\n");} return tEQUALS;} 
"<" {if(isToken){printf("tLESS\n");} return '<';}
">" {if(isToken){printf("tGREATER\n");} return '>';}
"=" {if(isToken){printf("tASSIGN\n");} return '=';}
"!" {if(isToken){printf("tBANG\n");} return '!';} 
"!=" {if(isToken){printf("tNOTEQUALS\n");} return tNOTEQUALS;} 
"<=" {if(isToken){printf("tLESSEQUALS\n");} return tLESSEQUALS;} 
">=" {if(isToken){printf("tGREATEREQUALS\n");} return tGREATEREQUALS;} 
":=" {if(isToken){printf("tCOLONASSIGN\n");} return tCOLONASSIGN;} 
"..." {if(isToken){printf("tDOTS\n");} return tDOTS;} 
"(" {if(isToken){printf("tLEFTPAREN\n");} return '(';}
")" {if(isToken){printf("tRIGHTPAREN\n");} requiresIns = 1; return ')';}
"{" {if(isToken){printf("tLEFTBRACE\n");} return '{';}
"}" {if(isToken){printf("tRIGHTBRACE\n");} requiresIns = 1; return '}';}
"[" {if(isToken){printf("tLEFTBRACKET\n");} return '[';}
"]" {if(isToken){printf("tRIGHTBRACKET\n");} requiresIns = 1; return ']';}
":" {if(isToken){printf("tCOLON\n");} return ':';} 
";" {if(isToken){printf("tSEMICOLON\n");} requiresIns = 0; return ';';} //remove marker for semi-colon insertion
"," {if(isToken){printf("tCOMMA\n");} return ',';} 
"." {if(isToken){printf("tPERIOD\n");} return '.';} 


"break" { // KEYWORDS
	if(isToken){printf("tBREAK\n");} requiresIns = 1; return tBREAK;}
"case" {if(isToken){printf("tCASE\n");} return tCASE;}
"chan" {if(isToken){printf("tCHAN\n");} return tCHAN;}
"const" {if(isToken){printf("tCONST\n");} return tCONST;}
"continue" {if(isToken){printf("tCONTINUE\n");} requiresIns = 1; return tCONTINUE;}
"default" {if(isToken){printf("tDEFAULT\n");} return tDEFAULT;}
"defer" {if(isToken){printf("tDEFER\n");} return tDEFER;}
"else" {if(isToken){printf("tELSE\n");} return tELSE;}
"fallthrough" {if(isToken){printf("tFALLTHROUGH\n");} requiresIns = 1; return tFALLTHROUGH;}
"for" {if(isToken){printf("tFOR\n");} return tFOR;}
"func" {if(isToken){printf("tFUNC\n");} return tFUNC;}
"go" {if(isToken){printf("tGO\n");} return tGO;}
"goto" {if(isToken){printf("tGOTO\n");} return tGOTO;}
"if" {if(isToken){printf("tIF\n");} return tIF;}
"import" {if(isToken){printf("tIMPORT\n");} return tIMPORT;}
"interface" {if(isToken){printf("tINTERFACE\n");} return tINTERFACE;}
"map" {if(isToken){printf("tMAP\n");} return tMAP;}
"package" {if(isToken){printf("tPACKAGE\n");} return tPACKAGE;}
"range" {if(isToken){printf("tRANGE\n");} return tRANGE;}
"return" {if(isToken){printf("tRETURN\n");} requiresIns = 1; return tRETURN;}
"select" {if(isToken){printf("tSELECT\n");} return tSELECT;}
"struct" {if(isToken){printf("tSTRUCT\n");} return tSTRUCT;}
"switch" {if(isToken){printf("tSWITCH\n");} return tSWITCH;}
"type" {if(isToken){printf("tTYPE\n");} return tTYPE;}
"var" {if(isToken){printf("tVAR\n");} return tVAR;}


"//".* {//line comments
	}
"/*" .* "*/" {//block comments
	}


0|([1-9][0-9]*) { // INT
	//DECIMAL VALUE
    yylval.intval = atoi(yytext);
    if(isToken){
        fprintf(stdout, "tINTDECVAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tINTDECVAL;
}
0[0-7]* {//OCTAL VALUE
    yylval.strval = strdup(yytext);
    if(isToken){
        fprintf(stdout, "tINTOCTVAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tINTOCTVAL;
}
0(x|X)[0-9a-f]* {//HEXADECIMAL VALUE
    yylval.strval = strdup(yytext);
    if(isToken){
        fprintf(stdout, "tINTHEXVAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tINTHEXVAL;
}


[1-9][0-9]*.[0-9]* { // FLOAT
	//LEFT of period
    yylval.floatval = atof(yytext);
    if(isToken){
        fprintf(stdout, "tFLOATVAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tFLOATVAL;
}
([1-9][0-9]*)*.[0-9][0-9]* {//RIGHT of period
    yylval.floatval = atof(yytext);
    if(isToken){
        fprintf(stdout, "tFLOATVAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tFLOATVAL;
}


[ a-zA-Z0-9~@#%*/_`.:;!]|{PUNCTUATION}|\\\\|\{|\}|\[|\]|\(|\)|(\\a)|(\\b)|(\\f)|(\\n)|(\\r)|(\\t)|(\\v)|(\")|(\\')' 
{//RUNE (CHAR)
    yylval.identifier = strdup(yytext);
    if(isToken){
        fprintf(stdout, "tRUNEVAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tRUNEVAL;
}


\"([ a-zA-Z0-9~@#%\*/'_`.:;!]|{PUNCTUATION}|\\\\|\{|\}|\[|\]|\(|\)|(\\a)|(\\b)|(\\f)|(\\n)|(\\r)|(\\t)|(\\v)|(\\\"))*\"
{//STRING
    yylval.identifier = strdup(yytext);
    if(isToken){
        fprintf(stdout, "tSTRINGVAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tSTRINGVAL;
}

`([ a-zA-Z0-9~@#%\*/'_.:;!]|{PUNCTUATION}|\\|\{|\}|\[|\]|\(|\)|(\"))*`    
{//RAW STRING
    yylval.identifier = strdup(yytext);
    if(isToken){
        fprintf(stdout, "tRAWSTRINGVAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tRAWSTRINGVAL;

}



. {
    fprintf(stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext);
    exit(1);
}
%%
